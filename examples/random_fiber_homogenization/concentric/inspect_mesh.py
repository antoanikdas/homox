"""Verify and plot the supplied concentric random-fiber mesh (NumPy/SciPy/Matplotlib)."""

from collections import defaultdict
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.collections import PolyCollection
from scipy.io import netcdf_file


DIRECTORY = Path(__file__).resolve().parent
MESH = DIRECTORY / "mesh_in.e"


def main(mesh_path=MESH, expected_fibers=22, radius=0.06, make_plot=True, periodic=True):
    with netcdf_file(mesh_path, "r", mmap=False) as mesh:
        points = np.column_stack(
            (mesh.variables["coordx"][:].copy(), mesh.variables["coordy"][:].copy())
        )
        ids = mesh.variables["eb_prop1"][:].copy()
        blocks = {
            int(block_id): mesh.variables[f"connect{index}"][:].copy().astype(int) - 1
            for index, block_id in enumerate(ids, start=1)
        }

    edges = defaultdict(list)
    elements = []
    element_blocks = []
    areas = {}
    for block_id, connectivity in blocks.items():
        if connectivity.shape[1] != 3:
            raise RuntimeError("Expected an all-TRI3 mesh")
        vertices = points[connectivity]
        next_vertices = np.roll(vertices, -1, axis=1)
        signed_areas = 0.5 * np.sum(
            vertices[:, :, 0] * next_vertices[:, :, 1]
            - vertices[:, :, 1] * next_vertices[:, :, 0],
            axis=1,
        )
        if np.any(signed_areas <= 0):
            raise RuntimeError("Nonpositive element area detected")
        areas[block_id] = float(signed_areas.sum())
        for nodes in connectivity:
            element_id = len(elements)
            elements.append(nodes)
            element_blocks.append(block_id)
            for first, second in zip(nodes, np.roll(nodes, -1)):
                edges[tuple(sorted((int(first), int(second))))].append(element_id)

    adjacency = defaultdict(set)
    exterior = defaultdict(list)
    xmin, ymin = points.min(axis=0)
    xmax, ymax = points.max(axis=0)
    for edge, owners in edges.items():
        if len(owners) == 2:
            first, second = owners
            adjacency[first].add(second)
            adjacency[second].add(first)
        elif len(owners) == 1:
            endpoints = points[list(edge)]
            on_boundary = any(
                np.allclose(endpoints[:, axis], bound, atol=1e-12, rtol=0)
                for axis, bound in ((0, xmin), (0, xmax), (1, ymin), (1, ymax))
            )
            if not on_boundary:
                raise RuntimeError("Unstitched internal edge detected")
            for axis, bound, side in ((0, xmin, 0), (0, xmax, 1), (1, ymin, 2), (1, ymax, 3)):
                if np.allclose(endpoints[:, axis], bound, atol=1e-12, rtol=0):
                    interval = tuple(sorted(endpoints[:, 1 - axis]))
                    exterior[side].append((interval, owners[0]))
        else:
            raise RuntimeError("Nonmanifold edge detected")

    visited = {0}
    pending = [0]
    while pending:
        for neighbor in adjacency[pending.pop()] - visited:
            visited.add(neighbor)
            pending.append(neighbor)
    if len(visited) != len(elements):
        raise RuntimeError("Disconnected mesh detected")

    for axis, low, high in ((0, xmin, xmax), (1, ymin, ymax)):
        lower = np.sort(points[np.isclose(points[:, axis], low), 1 - axis])
        upper = np.sort(points[np.isclose(points[:, axis], high), 1 - axis])
        if lower.shape != upper.shape or not np.allclose(lower, upper, atol=1e-12, rtol=0):
            raise RuntimeError("Opposite exterior boundary nodes do not match")

    if not np.isclose(sum(areas.values()), (xmax - xmin) * (ymax - ymin), atol=1e-12):
        raise RuntimeError("Mesh area does not match the rectangular domain")
    if not np.isclose(areas[1], expected_fibers * np.pi * radius**2, atol=1e-10, rtol=0):
        raise RuntimeError("Fiber area does not match the expected periodic disk area")
    if periodic:
        for first_side, second_side in ((0, 1), (2, 3)):
            first_edges = sorted(exterior[first_side])
            second_edges = sorted(exterior[second_side])
            if len(first_edges) != len(second_edges):
                raise RuntimeError("Periodic edge counts differ")
            for (first_interval, first), (second_interval, second) in zip(first_edges, second_edges):
                if not np.allclose(first_interval, second_interval, atol=1e-10, rtol=0):
                    raise RuntimeError("Periodic edge intervals do not match")
                if element_blocks[first] != element_blocks[second]:
                    raise RuntimeError("Opposite boundary material phases do not match")
                adjacency[first].add(second)
                adjacency[second].add(first)
        remaining = {i for i, block in enumerate(element_blocks) if block == 1}
        components = 0
        while remaining:
            components += 1
            pending = [remaining.pop()]
            while pending:
                neighbors = adjacency[pending.pop()] & remaining
                remaining -= neighbors
                pending.extend(neighbors)
        if components != expected_fibers:
            raise RuntimeError(f"Expected {expected_fibers} periodic fibers, found {components}")
        crossing_sides = [sum(element_blocks[owner] == 1 for _, owner in exterior[side])
                          for side in range(4)]
        if not all(crossing_sides):
            raise RuntimeError("Verification case must include fiber segments on all four boundaries")
        print(f"Periodic fibers: {components}; fiber boundary segments (L/R/B/T): {crossing_sides}")
    print(f"Elements: {len(elements)}; nodes: {len(points)}")
    print(f"Fiber area fraction: {areas[1] / sum(areas.values()):.10f}")
    print("Verified: all TRI3, positive areas, shared internal edges, connectivity, total area, periodic nodes")

    if not make_plot:
        return

    fig, ax = plt.subplots(figsize=(9, 9), constrained_layout=True)
    colors = {1: "#357caf", 2: "#ffffff"}
    for block_id, connectivity in blocks.items():
        ax.add_collection(
            PolyCollection(
                points[connectivity], facecolors=colors[block_id],
                edgecolors="#4b5055", linewidths=0.18,
            )
        )
    ax.set(xlim=(xmin, xmax), ylim=(ymin, ymax), xlabel="x", ylabel="y")
    ax.set_aspect("equal")
    ax.set_title("Random fibers: conforming all-triangle mesh")
    fig.savefig(DIRECTORY / "mesh.png", dpi=220)
    plt.close(fig)


if __name__ == "__main__":
    main()
