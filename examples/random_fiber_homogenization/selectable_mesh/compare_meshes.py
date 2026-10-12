"""Verify identical sampled geometry and plot the two mesh representations."""

import json
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.collections import PolyCollection
import numpy as np
from scipy.io import netcdf_file


DIRECTORY = Path(__file__).resolve().parent
RADIUS = 0.06


def read_centers(path):
    data = json.loads(path.read_text())
    records = data["time_steps"][0]["geometry"]["values"]["MeshMetaData/rve/fiber_centers"]["value"]
    return np.array([[record["x"], record["y"]] for record in records])


def read_mesh(path):
    with netcdf_file(path, "r", mmap=False) as mesh:
        points = np.column_stack((mesh.variables["coordx"][:].copy(),
                                  mesh.variables["coordy"][:].copy()))
        blocks = {int(block): mesh.variables[f"connect{index}"][:].copy().astype(int) - 1
                  for index, block in enumerate(mesh.variables["eb_prop1"][:], start=1)}
    return points, blocks


def area(points, connectivity):
    vertices = points[connectivity]
    following = np.roll(vertices, -1, axis=1)
    signed = 0.5 * np.sum(vertices[:, :, 0] * following[:, :, 1]
                          - vertices[:, :, 1] * following[:, :, 0], axis=1)
    if np.any(signed <= 0):
        raise RuntimeError("Nonpositive element area")
    return float(signed.sum())


def main():
    centers = read_centers(DIRECTORY / "concentric_geometry.json")
    if centers.shape != (22, 2) or not np.array_equal(
        centers, read_centers(DIRECTORY / "structured_geometry.json")
    ):
        raise RuntimeError("Mesh choices produced different fiber centers")
    fig, axes = plt.subplots(1, 2, figsize=(15, 7), constrained_layout=True)
    summary = []
    for ax, mode in zip(axes, ("concentric", "structured")):
        points, blocks = read_mesh(DIRECTORY / f"{mode}.e")
        expected_vertices = 3 if mode == "concentric" else 4
        if any(connectivity.shape[1] != expected_vertices for connectivity in blocks.values()):
            raise RuntimeError("Unexpected or mixed element types")
        total = sum(area(points, connectivity) for connectivity in blocks.values())
        if not np.isclose(total, 1, atol=1e-12):
            raise RuntimeError("Unexpected domain area")
        fraction = area(points, blocks[1]) / total
        for block, connectivity in blocks.items():
            if mode == "structured":
                centroids = points[connectivity].mean(axis=1)
                distance = np.abs(centroids[:, None, :] - centers[None, :, :])
                distance = np.minimum(distance, 1 - distance)
                in_fiber = np.any(np.sum(distance**2, axis=2) <= RADIUS**2, axis=1)
                if not np.all(in_fiber == (block == 1)):
                    raise RuntimeError("Structured blocks do not match the shared fiber geometry")
            ax.add_collection(PolyCollection(points[connectivity],
                                              facecolors="#357caf" if block == 1 else "white",
                                              edgecolors="#555b60", linewidths=0.08))
        ax.set(xlim=(0, 1), ylim=(0, 1), xlabel="x", ylabel="y")
        ax.set_aspect("equal")
        count = sum(len(connectivity) for connectivity in blocks.values())
        ax.set_title(f"{mode.capitalize()}: {count:,} elements; fiber fraction {100*fraction:.4f}%")
        summary.append((mode, count, fraction))
    fig.savefig(DIRECTORY / "mesh_comparison.png", dpi=220)
    plt.close(fig)
    print("Identical centers verified for all 22 fibers; structured phase classification verified.")
    for mode, count, fraction in summary:
        print(f"{mode}: {count} elements, fiber fraction {fraction:.10f}")


if __name__ == "__main__":
    main()
