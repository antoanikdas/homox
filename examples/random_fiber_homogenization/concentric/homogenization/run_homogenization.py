"""Run the five periodic elastic load cases and reconstruct effective stiffness."""

import csv
from pathlib import Path
import re
import subprocess
import time

import numpy as np


DIRECTORY = Path(__file__).resolve().parent
EXECUTABLE = DIRECTORY.parents[3] / "homox-opt"
RUN_CASES = True
CASES = (("inplane", "xx"), ("inplane", "yy"), ("inplane", "xy"),
         ("antiplane", "xz"), ("antiplane", "yz"))


def final_row(path):
    with path.open(newline="") as handle:
        rows = list(csv.DictReader(handle))
    if not rows:
        raise RuntimeError(f"No result rows in {path}")
    return {name: float(value) for name, value in rows[-1].items()}


def matrix_text(matrix):
    return "```text\n" + "\n".join(
        "[ " + "  ".join(f"{value: .9f}" for value in row) + " ]" for row in matrix
    ) + "\n```\n"


def main():
    rows = {}
    stats = {}
    for formulation, case in CASES:
        folder = DIRECTORY / formulation
        log_path = folder / f"{case}.log"
        if RUN_CASES:
            print(f"Running {formulation}/{case}", flush=True)
            start = time.monotonic()
            with log_path.open("w") as log:
                completed = subprocess.run(
                    [str(EXECUTABLE), "-i", f"{case}.i"], cwd=folder,
                    stdout=log, stderr=subprocess.STDOUT, check=False,
                )
            if completed.returncode:
                raise RuntimeError(f"Solve failed; see {log_path}")
            print(f"Completed in {time.monotonic() - start:.2f} s", flush=True)
        log = re.sub(r"\x1b\[[0-9;]*m", "", log_path.read_text())
        if "Solve Converged!" not in log:
            raise RuntimeError(f"No convergence confirmation in {log_path}")
        nonlinear = re.findall(r"(\d+) Nonlinear \|R\| = ([\deE+.-]+)", log)
        linear = re.findall(r"(\d+) Linear \|R\|", log)
        stats[case] = (int(nonlinear[-1][0]), max(map(int, linear), default=0),
                       float(nonlinear[-1][1]))
        rows[case] = final_row(folder / f"{case}_out.csv")

    strain = np.column_stack([
        [rows[case]["exx"], rows[case]["eyy"], 2 * rows[case]["exy"]]
        for case in ("xx", "yy", "xy")
    ])
    stress = np.column_stack([
        [rows[case]["sxx"], rows[case]["syy"], rows[case]["sxy"]]
        for case in ("xx", "yy", "xy")
    ])
    gradient = np.column_stack([
        [rows[case]["gamma_xz"], rows[case]["gamma_yz"]] for case in ("xz", "yz")
    ])
    traction = np.column_stack([
        [rows[case]["tau_xz"], rows[case]["tau_yz"]] for case in ("xz", "yz")
    ])
    stiffness = np.linalg.solve(strain.T, stress.T).T
    antiplane = np.linalg.solve(gradient.T, traction.T).T
    fiber_fraction = rows["xx"]["fiber_area"] / rows["xx"]["area"]
    if not np.isclose(fiber_fraction, 22 * np.pi * 0.06**2, atol=1e-10):
        raise RuntimeError("Unexpected fiber fraction; check the mesh and example settings")

    symmetry = {}
    for name, matrix in (("in-plane", stiffness), ("antiplane", antiplane)):
        symmetry[name] = np.linalg.norm(matrix - matrix.T) / np.linalg.norm(matrix)
        if symmetry[name] > 1e-8:
            raise RuntimeError(f"Unexpected {name} stiffness asymmetry")
        if np.linalg.eigvalsh(0.5 * (matrix + matrix.T)).min() <= 0:
            raise RuntimeError(f"The {name} effective stiffness is not positive definite")

    energy_errors = {}
    for formulation, case in CASES:
        row = rows[case]
        if formulation == "inplane":
            macro_energy = 0.5 * (row["sxx"] * row["exx"] + row["syy"] * row["eyy"]
                                  + 2 * row["sxy"] * row["exy"])
        else:
            macro_energy = 0.5 * (row["tau_xz"] * row["gamma_xz"]
                                  + row["tau_yz"] * row["gamma_yz"])
        micro_energy = row["energy"] / row["area"]
        energy_errors[case] = abs(micro_energy - macro_energy) / abs(macro_energy)
        if energy_errors[case] > 1e-8:
            raise RuntimeError(f"Energy consistency check failed for {case}")

    matrix_phase = np.array([[120., 40., 0.], [40., 120., 0.], [0., 0., 40.]])
    voigt = (1 + 9 * fiber_fraction) * matrix_phase
    reuss = matrix_phase / (1 - fiber_fraction + fiber_fraction / 10)
    symmetric = 0.5 * (stiffness + stiffness.T)
    reuss_margin = np.linalg.eigvalsh(symmetric - reuss).min()
    voigt_margin = np.linalg.eigvalsh(voigt - symmetric).min()
    if min(reuss_margin, voigt_margin) < -1e-8:
        raise RuntimeError("Effective stiffness violates the Voigt/Reuss bounds")

    lines = ["# Periodic Random-Fiber Homogenization", "",
             "All five cases use the same saved `../mesh_in.e`: 22 radius-0.06 fibers,",
             "a unit-square domain, 13,710 TRI3 elements, and fitted periodic interfaces.",
             f"Fiber fraction: **{100 * fiber_fraction:.8f}%**.", "",
             "Matrix: E=100, nu=0.25, G=40. Fiber: E=1000, nu=0.25, G=400.",
             "Stiffness and stress units are the units chosen for E; strains are dimensionless.", "",
             "## Formulation", "",
             "The in-plane calculation is **plane strain**, with epsilon_zz=epsilon_xz=epsilon_yz=0.",
             "Displacements are an affine macro part plus periodic fluctuations; average strains",
             "are imposed through the Homox scalar homogenization equations. One corner fixes",
             "the fluctuation translation. The antiplane calculation solves a periodic disp_z",
             "fluctuation with two prescribed macro gradients. Both use NEWTON and SuperLU_DIST.", "",
             "## Effective Matrices", "",
             "In-plane order: stress=(sigma_xx, sigma_yy, sigma_xy),",
             "strain=(epsilon_xx, epsilon_yy, gamma_xy), with gamma_xy=2*epsilon_xy.",
             "Normal loading amplitude is 0.01; shear loading uses epsilon_xy=0.005.",
             "Measured columns satisfy C_eff = stress_columns * inverse(strain_columns).", "",
             matrix_text(stiffness),
             "Antiplane order: stress=(tau_xz, tau_yz), strain=(gamma_xz, gamma_yz).",
             "Each prescribed engineering-shear amplitude is 0.01. K_eff is reconstructed",
             "from the two measured stress and gradient columns.", "", matrix_text(antiplane),
             "These are reduced in-plane and antiplane matrices, not a full 3D 6-by-6 tensor.",
             "A finite random realization need not be isotropic: normal-shear and antiplane",
             "cross-couplings are retained rather than forced to zero.", "",
             "## Load Results", "",
             "| Case | Average stress components | Average energy density | Relative energy mismatch |",
             "| --- | --- | ---: | ---: |"]
    for formulation, case in CASES:
        keys = ("sxx", "syy", "sxy") if formulation == "inplane" else ("tau_xz", "tau_yz")
        values = ", ".join(f"{rows[case][key]:.9f}" for key in keys)
        lines.append(f"| {case} | {values} | {rows[case]['energy'] / rows[case]['area']:.10f} "
                     f"| {energy_errors[case]:.3e} |")
    lines += ["", "Energy is integrated directly from the AD elastic material property and divided",
              "by domain area. The comparison is with 0.5 * average_stress : average_strain",
              "(or 0.5 * average_traction dot average_gradient for antiplane).", "",
              "## Verification", "",
              f"Relative in-plane symmetry error: {symmetry['in-plane']:.3e}.",
              f"Relative antiplane symmetry error: {symmetry['antiplane']:.3e}.",
              "Both symmetric matrices have positive eigenvalues.",
              f"Minimum eigenvalue of C_eff - C_Reuss: {reuss_margin:.9f}.",
              f"Minimum eigenvalue of C_Voigt - C_eff: {voigt_margin:.9f}.", "",
              "| Case | Newton iterations | Maximum Krylov iteration index per solve | Final nonlinear residual |",
              "| --- | ---: | ---: | ---: |"]
    for _, case in CASES:
        nonlinear, linear, residual = stats[case]
        lines.append(f"| {case} | {nonlinear} | {linear} | {residual:.3e} |")
    lines += ["", "## Reproduce", "", "From this directory, with the MOOSE environment activated:",
              "", "```bash", "python run_homogenization.py", "```", "",
              "The script runs inputs sequentially and saves CSV, Exodus, and logs in the",
              "`inplane` and `antiplane` directories. Set `RUN_CASES = False` to reconstruct",
              "this report from existing results without repeating the solves."]
    (DIRECTORY / "RESULTS.md").write_text("\n".join(lines) + "\n")
    np.set_printoptions(precision=9, suppress=True)
    print("C_eff (plane strain, engineering shear):\n", stiffness)
    print("K_eff (antiplane):\n", antiplane)
    print("Report:", DIRECTORY / "RESULTS.md")


if __name__ == "__main__":
    main()
