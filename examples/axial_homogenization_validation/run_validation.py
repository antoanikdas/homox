"""Verify all six transverse BC families using a homogeneous axial-capable RVE."""

import csv
from pathlib import Path
import re
import subprocess

import numpy as np


DIRECTORY = Path(__file__).resolve().parent
EXECUTABLE = DIRECTORY.parents[1] / "homox-opt"
RUN_CASES = True
FAMILIES = ("periodic_kinematic", "periodic_mixed", "periodic_static",
            "nonperiodic_kinematic", "nonperiodic_mixed", "nonperiodic_static")
LOADS = ("xx", "yy", "zz", "xy")
EXACT = np.array([[120., 40., 40., 0.], [40., 120., 40., 0.],
                  [40., 40., 120., 0.], [0., 0., 0., 40.]])


def analytical_response(family, load):
    if family.endswith("kinematic"):
        strain_control = [True, True, True, True]
    elif family.endswith("static"):
        strain_control = [False, False, True, False]
    elif load in ("xx", "zz"):
        strain_control = [True, False, True, False]
    elif load == "yy":
        strain_control = [False, True, True, False]
    else:
        strain_control = [False, False, True, True]
    component = LOADS.index(load)
    target = np.zeros(4)
    target[component] = 0.01 if strain_control[component] else 1.0
    system = EXACT.copy()
    for index, is_strain in enumerate(strain_control):
        if is_strain:
            system[index] = np.eye(4)[index]
    strain = np.linalg.solve(system, target)
    stress = EXACT @ strain
    return strain, stress


def main():
    results = {}
    for family in FAMILIES:
        strains, stresses, rows, stats = [], [], {}, {}
        for load in LOADS:
            folder = DIRECTORY / family
            log_path = folder / f"{load}.log"
            if RUN_CASES:
                print(f"Running {family}/{load}", flush=True)
                with log_path.open("w") as log:
                    completed = subprocess.run([str(EXECUTABLE), "-i", f"{load}.i"],
                                               cwd=folder, stdout=log, stderr=subprocess.STDOUT)
                if completed.returncode:
                    raise RuntimeError(f"Solve failed; inspect {log_path}")
            with (folder / f"{load}_out.csv").open(newline="") as handle:
                row = {key: float(value) for key, value in list(csv.DictReader(handle))[-1].items()}
            e = np.array([row["exx"], row["eyy"], row["ezz"], 2*row["exy"]])
            s = np.array([row["sxx"], row["syy"], row["szz"], row["sxy"]])
            expected_e, expected_s = analytical_response(family, load)
            if not np.allclose(e, expected_e, atol=1e-11, rtol=1e-9):
                raise RuntimeError(f"Incorrect strain for {family}/{load}: {e} != {expected_e}")
            if not np.allclose(s, expected_s, atol=1e-9, rtol=1e-9):
                raise RuntimeError(f"Incorrect stress for {family}/{load}: {s} != {expected_s}")
            energy = row["energy"] / row["area"]
            expected_energy = 0.5 * expected_s @ expected_e
            if not np.isclose(energy, expected_energy, atol=1e-11, rtol=1e-9):
                raise RuntimeError(f"Incorrect energy for {family}/{load}")
            log = re.sub(r"\x1b\[[0-9;]*m", "", log_path.read_text())
            if "Solve Converged!" not in log:
                raise RuntimeError(f"Missing convergence confirmation for {family}/{load}")
            nonlinear = re.findall(r"(\d+) Nonlinear \|R\| = ([\deE+.-]+)", log)
            stats[load] = (int(nonlinear[-1][0]), float(nonlinear[-1][1]))
            strains.append(e)
            stresses.append(s)
            rows[load] = (e, s, energy)
        recovered = np.linalg.solve(np.array(strains), np.array(stresses)).T
        error = np.linalg.norm(recovered - EXACT) / np.linalg.norm(EXACT)
        symmetry = np.linalg.norm(recovered - recovered.T) / np.linalg.norm(recovered)
        eigenvalues = np.linalg.eigvalsh(0.5 * (recovered + recovered.T))
        if error > 1e-9 or symmetry > 1e-9 or eigenvalues.min() <= 0:
            raise RuntimeError(f"Stiffness recovery failed for {family}")
        results[family] = (recovered, error, symmetry, eigenvalues, rows, stats)

    lines = ["# Homogeneous Axial Verification: All Six BC Families", "",
             "Unit-square 4 x 4 QUAD4 cross-section, homogeneous isotropic E=100, nu=0.25.",
             "All 24 solves use NEWTON and SuperLU_DIST. The fiber direction is z.", "",
             "## Exact Matrix", "",
             "Order: (xx, yy, zz, xy), with engineering shear gamma_xy=2*epsilon_xy.",
             "The 4-by-4 generalized-plane-strain block is:", "", "```text",
             "[120  40  40   0]", "[ 40 120  40   0]", "[ 40  40 120   0]",
             "[  0   0   0  40]", "```", "",
             "Thus C13=C23=40, C33=120, and C36=0 for every BC family.", "",
             "## BC Definitions", "",
             "Periodic cases solve affine macro strain plus periodic displacement fluctuations.",
             "Kinematic imposes all four average strains. Mixed imposes the loaded transverse",
             "strain while relaxing the complementary transverse stresses. For the zz mixed",
             "case, epsilon_xx=0, sigma_yy=sigma_xy=0, and epsilon_zz=0.01.",
             "Static prescribes all three transverse average stresses, while epsilon_zz is imposed.", "",
             "Nonperiodic kinematic prescribes affine displacement on the complete outer boundary.",
             "Nonperiodic mixed prescribes opposing x or y displacements with complementary",
             "traction-free directions; its xy case uses top/bottom x-displacement and fixed",
             "left/right y-displacement. Nonperiodic static applies uniform transverse boundary",
             "tractions and removes only rigid translations and rotation with corner pins.", "",
             "For nonperiodic cases, ADHomoxCompatibleSmallStrain supplies uniform epsilon_zz",
             "through out_of_plane_strain. The name static refers to transverse control; no",
             "physical z-end faces exist on this 2D mesh. These are axial-strain-controlled",
             "generalized-plane-strain tests, not fully 3D uniform-traction tests.", "",
             "## Axial Load Response", "",
             "All zz cases impose epsilon_zz=0.01. Their transverse restraints differ:", "",
             "| Family | epsilon_xx | epsilon_yy | sigma_xx | sigma_yy | sigma_zz | Energy density |",
             "| --- | ---: | ---: | ---: | ---: | ---: | ---: |"]
    for family in FAMILIES:
        e, s, energy = results[family][4]["zz"]
        lines.append(f"| {family} | {e[0]:.9f} | {e[1]:.9f} | {s[0]:.9f} | {s[1]:.9f} "
                     f"| {s[2]:.9f} | {energy:.9f} |")
    lines += ["", "The direct ratio sigma_zz/epsilon_zz is 120 for kinematic, 106.666667 for",
              "mixed, and 100 for transverse static relaxation. These are different restraint",
              "responses, not conflicting estimates of C33. The matrix is recovered from all",
              "four measured strain/stress columns: C = Sigma * inverse(Epsilon).", "",
              "## Recovered Axial Terms", "",
              "| Family | C13 | C23 | C33 | C36 | Relative full-block error |",
              "| --- | ---: | ---: | ---: | ---: | ---: |"]
    for family in FAMILIES:
        C, error = results[family][:2]
        lines.append(f"| {family} | {C[0,2]:.9f} | {C[1,2]:.9f} | {C[2,2]:.9f} "
                     f"| {C[2,3]:.3e} | {error:.3e} |")
    lines += ["", "All strain, stress, and energy outputs match the analytical solution for each",
              "control condition. Recovered matrices are symmetric and positive definite.", "",
              "## Convergence", "",
              "| Family | Load | Newton iterations | Final residual |",
              "| --- | --- | ---: | ---: |"]
    for family in FAMILIES:
        for load in LOADS:
            iterations, residual = results[family][5][load]
            lines.append(f"| {family} | {load} | {iterations} | {residual:.3e} |")
    lines += ["", "## Reproduce", "", "Activate the MOOSE environment, then run:", "",
              "```bash", "python run_validation.py", "```", "",
              "Set RUN_CASES=False to rebuild the report from existing outputs.",
              "CSV, Exodus, and logs remain in each BC family's subdirectory."]
    (DIRECTORY / "RESULTS.md").write_text("\n".join(lines) + "\n")
    for family in FAMILIES:
        print(f"{family}: relative matrix error {results[family][1]:.3e}")
    print("Verified all 24 cases. Report:", DIRECTORY / "RESULTS.md")


if __name__ == "__main__":
    main()
