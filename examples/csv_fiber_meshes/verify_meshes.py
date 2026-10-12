"""Check the CSV geometry representations and save a side-by-side mesh image."""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.collections import PolyCollection
import numpy as np
from scipy.io import netcdf_file


DIRECTORY = Path(__file__).resolve().parent


def main():
    with (DIRECTORY / "inclusions.csv").open(newline="") as stream:
        inclusions = list(csv.DictReader(stream))
    centers = np.array([[float(row["x"]), float(row["y"])] for row in inclusions])
    radii = np.array([float(row["radius"]) for row in inclusions])
    nominal_area = np.pi * np.sum(radii**2)
    figure, axes = plt.subplots(1, 2, figsize=(13, 6), constrained_layout=True)
    for axis, mode, vertices in zip(axes, ("structured", "triangular"), (4, 3)):
        with netcdf_file(DIRECTORY / f"{mode}.e", "r", mmap=False) as mesh:
            points = np.column_stack((mesh.variables["coordx"][:].copy(),
                                      mesh.variables["coordy"][:].copy()))
            blocks = {int(block): mesh.variables[f"connect{index}"][:].copy().astype(int) - 1
                      for index, block in enumerate(mesh.variables["eb_prop1"][:], start=1)}
        total_area = 0.0
        fiber_area = 0.0
        for block, connectivity in blocks.items():
            if connectivity.shape[1] != vertices:
                raise RuntimeError("Unexpected or mixed element types")
            coordinates = points[connectivity]
            following = np.roll(coordinates, -1, axis=1)
            areas = 0.5 * np.sum(coordinates[:, :, 0] * following[:, :, 1] -
                                 coordinates[:, :, 1] * following[:, :, 0], axis=1)
            if np.any(areas <= 0):
                raise RuntimeError("Nonpositive element area")
            total_area += areas.sum()
            if block == 1:
                fiber_area += areas.sum()
            if mode == "structured":
                distance = np.abs(coordinates.mean(axis=1)[:, None, :] - centers[None, :, :])
                distance = np.minimum(distance, 1 - distance)
                in_fiber = np.any(np.sum(distance**2, axis=2) <= radii[None, :]**2, axis=1)
                if not np.all(in_fiber == (block == 1)):
                    raise RuntimeError("Structured phases do not match CSV centers and radii")
            axis.add_collection(PolyCollection(coordinates,
                                               facecolors="#357caf" if block == 1 else "white",
                                               edgecolors="#555b60", linewidths=0.1))
        if not np.isclose(total_area, 1, rtol=0, atol=1e-11):
            raise RuntimeError("Incorrect domain area")
        if mode == "triangular" and not np.isclose(fiber_area, nominal_area, rtol=1e-10):
            raise RuntimeError("Fitted fiber areas do not match CSV radii")
        if mode == "triangular":
            for direction in range(2):
                low = np.sort(points[np.isclose(points[:, direction], 0, atol=1e-11), 1-direction])
                high = np.sort(points[np.isclose(points[:, direction], 1, atol=1e-11), 1-direction])
                if low.shape != high.shape or not np.allclose(low, high, rtol=0, atol=1e-11):
                    raise RuntimeError("Opposite periodic boundary nodes do not match")
        count = sum(len(connectivity) for connectivity in blocks.values())
        axis.set(xlim=(0, 1), ylim=(0, 1), xlabel="x", ylabel="y",
                 title=f"{mode.capitalize()}: {count:,} elements; VF = {100*fiber_area:.3f}%")
        axis.set_aspect("equal")
        print(f"{mode}: {count} elements, fiber fraction = {fiber_area:.12f}")
    figure.savefig(DIRECTORY / "mesh_comparison.png", dpi=220)
    plt.close(figure)
    print(f"CSV nominal fiber fraction = {nominal_area:.12f}; geometry checks passed.")


if __name__ == "__main__":
    main()
