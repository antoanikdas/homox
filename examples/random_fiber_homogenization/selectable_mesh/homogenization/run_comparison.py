"""Homogenize both saved representations of the same periodic fiber geometry."""

import csv
import json
from pathlib import Path
import re
import subprocess
import time

import numpy as np


DIRECTORY = Path(__file__).resolve().parent
MESH_DIRECTORY = DIRECTORY.parent
EXECUTABLE = DIRECTORY.parents[3] / "homox-opt"
RUN_CASES = True
MODES = ("concentric", "structured")
CASES = (("inplane", "xx"), ("inplane", "yy"), ("inplane", "xy"),
         ("antiplane", "xz"), ("antiplane", "yz"))


def read_centers(mode):
    data = json.loads((MESH_DIRECTORY / f"{mode}_geometry.json").read_text())
    return data["time_steps"][0]["geometry"]["values"]["MeshMetaData/rve/fiber_centers"]["value"]


def matrix_text(matrix):
    return "```text\n" + "\n".join(
        "[ " + "  ".join(f"{value: .9f}" for value in row) + " ]" for row in matrix
    ) + "\n```"


def phase_stiffness():
    return np.array([[120., 40., 0.], [40., 120., 0.], [0., 0., 40.]])


def reconstruct(rows):
    strain = np.column_stack([[rows[c]["exx"], rows[c]["eyy"], 2*rows[c]["exy"]]
                              for c in ("xx", "yy", "xy")])
    stress = np.column_stack([[rows[c]["sxx"], rows[c]["syy"], rows[c]["sxy"]]
                              for c in ("xx", "yy", "xy")])
    gradient = np.column_stack([[rows[c]["gamma_xz"], rows[c]["gamma_yz"]]
                                for c in ("xz", "yz")])
    traction = np.column_stack([[rows[c]["tau_xz"], rows[c]["tau_yz"]]
                                for c in ("xz", "yz")])
    return np.linalg.solve(strain.T, stress.T).T, np.linalg.solve(gradient.T, traction.T).T


def main():
    centers = read_centers("concentric")
    if len(centers) != 22 or centers != read_centers("structured"):
        raise RuntimeError("The two mesh representations must have exactly identical fiber centers")
    results = {}
    for mode in MODES:
        rows, stats = {}, {}
        for formulation, case in CASES:
            output = DIRECTORY / "results" / mode / formulation
            output.mkdir(parents=True, exist_ok=True)
            file_base = output / f"{case}_out"
            log_path = output / f"{case}.log"
            if RUN_CASES:
                print(f"Running {mode}/{formulation}/{case}", flush=True)
                start = time.monotonic()
                with log_path.open("w") as log:
                    command = [str(EXECUTABLE), "-i", f"{case}.i",
                               f"Mesh/rve/file={MESH_DIRECTORY / (mode + '.e')}",
                               f"Outputs/file_base={file_base}"]
                    completed = subprocess.run(command, cwd=DIRECTORY / formulation,
                                               stdout=log, stderr=subprocess.STDOUT)
                if completed.returncode:
                    raise RuntimeError(f"Solve failed; inspect {log_path}")
                print(f"Completed in {time.monotonic() - start:.2f} s", flush=True)
            with file_base.with_suffix(".csv").open(newline="") as handle:
                final = list(csv.DictReader(handle))[-1]
            rows[case] = {name: float(value) for name, value in final.items()}
            log = re.sub(r"\x1b\[[0-9;]*m", "", log_path.read_text())
            if "Solve Converged!" not in log:
                raise RuntimeError(f"Missing convergence confirmation in {log_path}")
            nonlinear = re.findall(r"(\d+) Nonlinear \|R\| = ([\deE+.-]+)", log)
            linear = re.findall(r"(\d+) Linear \|R\|", log)
            stats[case] = (int(nonlinear[-1][0]), max(map(int, linear), default=0),
                           float(nonlinear[-1][1]))

        C, K = reconstruct(rows)
        fraction = rows["xx"]["fiber_area"] / rows["xx"]["area"]
        checks = {}
        for name, matrix in (("C", C), ("K", K)):
            asymmetry = np.linalg.norm(matrix - matrix.T) / np.linalg.norm(matrix)
            eigenvalues = np.linalg.eigvalsh(0.5 * (matrix + matrix.T))
            if asymmetry > 1e-8 or eigenvalues.min() <= 0:
                raise RuntimeError(f"Invalid {mode} {name}: symmetry or positivity check failed")
            checks[name] = (asymmetry, eigenvalues)
        reuss_factor = 1 / (1 - fraction + fraction/10)
        voigt_factor = 1 + 9*fraction
        C_symmetric = 0.5 * (C + C.T)
        bounds = (np.linalg.eigvalsh(C_symmetric - reuss_factor*phase_stiffness()).min(),
                  np.linalg.eigvalsh(voigt_factor*phase_stiffness() - C_symmetric).min(),
                  np.linalg.eigvalsh(0.5*(K+K.T) - 40*reuss_factor*np.eye(2)).min(),
                  np.linalg.eigvalsh(40*voigt_factor*np.eye(2) - 0.5*(K+K.T)).min())
        if min(bounds) < -1e-8:
            raise RuntimeError(f"The {mode} matrices violate Voigt-Reuss bounds")
        energy_errors = {}
        for formulation, case in CASES:
            row = rows[case]
            if not np.isclose(row["fiber_area"] / row["area"], fraction, atol=1e-12):
                raise RuntimeError("Geometry differs between load cases")
            if formulation == "inplane":
                macro = 0.5 * (row["sxx"]*row["exx"] + row["syy"]*row["eyy"]
                               + 2*row["sxy"]*row["exy"])
            else:
                macro = 0.5 * (row["tau_xz"]*row["gamma_xz"] + row["tau_yz"]*row["gamma_yz"])
            energy_errors[case] = abs(row["energy"] / row["area"] - macro) / abs(macro)
            if energy_errors[case] > 1e-8:
                raise RuntimeError(f"Energy consistency check failed for {mode}/{case}")
        results[mode] = dict(C=C, K=K, fraction=fraction, rows=rows, stats=stats,
                             checks=checks, bounds=bounds, energy_errors=energy_errors)

    reference, structured = results["concentric"], results["structured"]
    C_error = np.linalg.norm(structured["C"] - reference["C"]) / np.linalg.norm(reference["C"])
    K_error = np.linalg.norm(structured["K"] - reference["K"]) / np.linalg.norm(reference["K"])
    lines = ["# Concentric vs Structured Homogenization", "",
             "The same 22 periodic circular fibers (radius 0.06, seed 1980) are represented",
             "by a fitted all-TRI3 mesh and a structured 128 x 128 all-QUAD4 mesh.",
             "The exported fiber centers were checked for exact equality before the solves.", "",
             "Matrix E=100, fiber E=1000, nu=0.25 for both phases. Consequently G_matrix=40",
             "and G_fiber=400. Stiffness units follow E; strains are dimensionless.",
             "All cases use periodic kinematic constraints, NEWTON, and SuperLU_DIST.", "",
             "The in-plane problem is **plane strain**. The order is stress=(xx, yy, xy)",
             "and strain=(epsilon_xx, epsilon_yy, gamma_xy), with gamma_xy=2*epsilon_xy.",
             "Normal loads use strain 0.01; the xy load uses epsilon_xy=0.005.",
             "Antiplane uses (tau_xz, tau_yz) and engineering strains (gamma_xz, gamma_yz),",
             "each loaded to 0.01. Both matrices are reconstructed from measured stress",
             "and strain columns without suppressing normal-shear or cross-shear coupling.", "",
             "## Geometry", "", "| Mesh | Elements | Fiber fraction |",
             "| --- | ---: | ---: |",
             f"| Concentric TRI3 | 13,710 | {100*reference['fraction']:.8f}% |",
             f"| Structured QUAD4 | 16,384 | {100*structured['fraction']:.8f}% |", "",
             "The ideal circles coincide, but centroid assignment on the structured grid",
             "approximates phase area. The fitted mesh preserves nominal disk area.", ""]
    for mode in MODES:
        result = results[mode]
        lines += [f"## {mode.capitalize()} Matrices", "", "C_eff (plane strain):", "",
                  matrix_text(result["C"]), "", "K_eff (antiplane):", "", matrix_text(result["K"]), ""]
    lines += ["## Differences", "",
              "Differences below are structured minus concentric; the fitted mesh is the",
              "comparison reference, not an exact continuum solution.",
              f"Relative Frobenius difference in C_eff: **{100*C_error:.6f}%**.",
              f"Relative Frobenius difference in K_eff: **{100*K_error:.6f}%**.",
              "These norms use the reported engineering-shear matrix coordinates.", "",
              "| Component | Concentric | Structured | Signed difference | Relative difference |",
              "| --- | ---: | ---: | ---: | ---: |"]
    entries = (("C11", "C", 0, 0), ("C22", "C", 1, 1), ("C12", "C", 0, 1),
               ("C66", "C", 2, 2), ("C16", "C", 0, 2), ("C26", "C", 1, 2),
               ("K11", "K", 0, 0), ("K22", "K", 1, 1), ("K12", "K", 0, 1))
    for name, key, i, j in entries:
        first, second = reference[key][i, j], structured[key][i, j]
        relative = abs(second-first) / abs(first)
        lines.append(f"| {name} | {first:.9f} | {second:.9f} | {second-first:+.9f} | {100*relative:.6f}% |")
    lines += ["", "Relative percentages for small coupling entries can be large despite a small",
              "absolute change; the full matrices and norm differences provide context.", "",
              "## Energy and Convergence", "",
              "Energy is the integral of the AD elastic-energy property divided by domain area.",
              "It is checked against half the contraction of average stress and average strain.", "",
              "| Case | Concentric energy density | Structured energy density | Relative difference |",
              "| --- | ---: | ---: | ---: |"]
    for _, case in CASES:
        first = reference["rows"][case]["energy"] / reference["rows"][case]["area"]
        second = structured["rows"][case]["energy"] / structured["rows"][case]["area"]
        lines.append(f"| {case} | {first:.10f} | {second:.10f} | {100*abs(second-first)/abs(first):.6f}% |")
    lines += ["", "| Mesh | Case | Newton iterations | Maximum Krylov iteration index | Final residual | Energy consistency mismatch |",
              "| --- | --- | ---: | ---: | ---: | ---: |"]
    for mode in MODES:
        for _, case in CASES:
            nonlinear, linear, residual = results[mode]["stats"][case]
            mismatch = results[mode]["energy_errors"][case]
            lines.append(f"| {mode} | {case} | {nonlinear} | {linear} | {residual:.3e} | {mismatch:.3e} |")
    lines += ["", "## Matrix Checks", "",
              "| Mesh | Matrix | Relative symmetry error | Eigenvalues |",
              "| --- | --- | ---: | --- |"]
    for mode in MODES:
        for key in ("C", "K"):
            asymmetry, eigenvalues = results[mode]["checks"][key]
            lines.append(f"| {mode} | {key} | {asymmetry:.3e} | "
                         + ", ".join(f"{value:.6f}" for value in eigenvalues) + " |")
    lines += ["", "All symmetry, positive-definiteness, energy-consistency, and Voigt-Reuss",
              "bound checks passed. Bounds use each mesh's actually represented fiber fraction.", "",
              "## Reproduce", "", "With the MOOSE environment activated, from this directory:",
              "", "```bash", "python run_comparison.py", "```", "",
              "Results, Exodus files, and solver logs are separated under `results/concentric`",
              "and `results/structured`. The script uses the saved meshes one directory above.",
              "Set `RUN_CASES = False` to rebuild this report from existing results."]
    (DIRECTORY / "RESULTS.md").write_text("\n".join(lines) + "\n")
    np.set_printoptions(precision=9, suppress=True)
    for mode in MODES:
        print(mode, "C_eff:\n", results[mode]["C"], "\nK_eff:\n", results[mode]["K"])
    print(f"C difference: {100*C_error:.6f}%; K difference: {100*K_error:.6f}%")
    print("Report:", DIRECTORY / "RESULTS.md")


if __name__ == "__main__":
    main()
