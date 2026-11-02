import os
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


FILES = [
    r"D:\parallel\takacsa\beadandó\pthread\times.txt",
    r"D:\parallel\takacsa\beadandó\sequential\times.txt",
    r"D:\parallel\takacsa\beadandó\openmpi\times.txt",
    r"D:\parallel\takacsa\beadandó\openmp\times.txt",
]

OUT_DIR = Path("performance_charts")
OUT_DIR.mkdir(exist_ok=True)


def plain_number_formatter(x, pos):
    if x >= 1:
        return f"{int(x):,}".replace(",", " ")
    return f"{x:g}"


def apply_plain_axis_format(x_axis=True, y_axis=True):
    ax = plt.gca()

    if x_axis:
        ax.xaxis.set_major_formatter(FuncFormatter(plain_number_formatter))

    if y_axis:
        ax.yaxis.set_major_formatter(FuncFormatter(plain_number_formatter))


def read_times(path):
    rows = []

    with open(path, "r", encoding="utf-8", errors="replace") as file:
        for line in file:
            line = line.strip()

            if not line:
                continue

            parts = line.split(";")

            if len(parts) == 6:
                method, algorithm, n, thread_count, depth, time_value = parts

            elif len(parts) == 5:
                method, algorithm, n, thread_count, time_value = parts
                depth = "-1"

            else:
                continue

            rows.append([
                method,
                algorithm,
                int(float(n)),
                int(float(thread_count)),
                int(float(depth)),
                float(time_value)
            ])

    return pd.DataFrame(
        rows,
        columns=["method", "algorithm", "n", "threadCount", "depth", "time"]
    )


def save_chart(filename):
    path = OUT_DIR / filename
    plt.tight_layout()
    plt.savefig(path, dpi=220, bbox_inches="tight")
    plt.close()
    return path


def annotate_bars(ax, bars):
    for bar in bars:
        height = bar.get_height()

        if height > 0:
            ax.annotate(
                f"{height:.2f}s",
                xy=(bar.get_x() + bar.get_width() / 2, height),
                xytext=(0, 3),
                textcoords="offset points",
                ha="center",
                va="bottom",
                fontsize=8
            )



dataframes = []

for path in FILES:
    if os.path.exists(path):
        dataframes.append(read_times(path))
        print(f"Beolvasva: {path}")
    else:
        print(f"Nem található: {path}")

if not dataframes:
    print("Nincs beolvasható fájl.")
    exit()

df = pd.concat(dataframes, ignore_index=True)

df = df[df["time"] > 0].copy()

df = df.groupby(
    ["method", "algorithm", "n", "threadCount"],
    as_index=False
)["time"].min()

created_files = []


method_comparison_plots = [
    ("mergesort", 1_000_000_000, 4, "grafikon_01.png"),
    ("quicksort", 1_000_000_000, 4, "grafikon_02.png"),
]

for algorithm, n_value, thread_count, filename in method_comparison_plots:
    sequential_data = df[
        (df["method"] == "sequential") &
        (df["algorithm"] == algorithm) &
        (df["n"] == n_value)
    ]

    parallel_data = df[
        (df["method"] != "sequential") &
        (df["algorithm"] == algorithm) &
        (df["n"] == n_value) &
        (df["threadCount"] == thread_count)
    ]

    data = pd.concat([sequential_data, parallel_data], ignore_index=True)

    if data.empty:
        print(f"Nincs adat ehhez: {algorithm}, n = {n_value}")
        continue

    order = ["sequential", "pthread", "OpenMP", "OpenMPI"]
    data["method"] = pd.Categorical(data["method"], categories=order, ordered=True)
    data = data.sort_values("method")

    plt.figure(figsize=(9, 5))

    bars = plt.bar(
        data["method"].astype(str),
        data["time"]
    )

    title_n = f"{n_value:,}".replace(",", " ")

    plt.title(
        f"{algorithm}: futási idők összehasonlítása\n"
        f"n = {title_n}, párhuzamos megoldásoknál {thread_count} szál/folyamat"
    )
    plt.xlabel("Megoldás")
    plt.ylabel("Futási idő (másodperc)")
    plt.grid(axis="y", alpha=0.35)

    annotate_bars(plt.gca(), bars)

    created_files.append(save_chart(filename))


algorithm_inside_method_plots = [
    ("pthread", 1_000_000, 4, "grafikon_03.png"),
    ("OpenMP", 1_000_000, 4, "grafikon_04.png"),
]

for method, n_value, thread_count, filename in algorithm_inside_method_plots:
    data = df[
        (df["method"] == method) &
        (df["n"] == n_value) &
        (df["threadCount"] == thread_count)
    ].sort_values("algorithm")

    if data.empty:
        print(f"Nincs adat ehhez: {method}, n = {n_value}, threadCount = {thread_count}")
        continue

    plt.figure(figsize=(9, 5))

    bars = plt.bar(
        data["algorithm"],
        data["time"]
    )

    title_n = f"{n_value:,}".replace(",", " ")

    plt.title(
        f"{method}: algoritmusok összehasonlítása\n"
        f"n = {title_n}, szál/folyamat = {thread_count}"
    )
    plt.xlabel("Algoritmus")
    plt.ylabel("Futási idő (másodperc)")

    plt.yscale("log")
    apply_plain_axis_format(x_axis=False, y_axis=True)

    plt.grid(axis="y", which="both", alpha=0.35)

    annotate_bars(plt.gca(), bars)

    created_files.append(save_chart(filename))


size_plots = [
    ("pthread", "mergesort", 8, "grafikon_05.png"),
    ("pthread", "quicksort", 8, "grafikon_06.png"),
    ("OpenMP", "mergesort", 8, "grafikon_07.png"),
    ("OpenMP", "quicksort", 8, "grafikon_08.png"),
]

for method, algorithm, thread_count, filename in size_plots:
    data = df[
        (df["method"] == method) &
        (df["algorithm"] == algorithm) &
        (df["threadCount"] == thread_count)
    ].sort_values("n")

    if data.empty:
        print(f"Nincs adat ehhez: {method} {algorithm}, threadCount = {thread_count}")
        continue

    plt.figure(figsize=(9, 5))

    plt.plot(
        data["n"],
        data["time"],
        marker="o"
    )

    plt.title(
        f"{method} {algorithm}: futási idő tömbméret szerint\n"
        f"szál/folyamat = {thread_count}"
    )
    plt.xlabel("Tömb mérete")
    plt.ylabel("Futási idő (másodperc)")
    plt.xscale("log")
    plt.yscale("log")
    apply_plain_axis_format(x_axis=True, y_axis=True)

    plt.grid(True, which="both", alpha=0.35)

    created_files.append(save_chart(filename))

thread_plots = [
    ("pthread", "mergesort", 1_000_000_000, "grafikon_09.png"),
    ("pthread", "quicksort", 1_000_000_000, "grafikon_10.png"),
    ("OpenMP", "mergesort", 1_000_000_000, "grafikon_11.png"),
    ("OpenMP", "quicksort", 1_000_000_000, "grafikon_12.png"),
    ("OpenMPI", "mergesort", 1_000_000_000, "grafikon_13.png"),
]

for method, algorithm, n_value, filename in thread_plots:
    data = df[
        (df["method"] == method) &
        (df["algorithm"] == algorithm) &
        (df["n"] == n_value)
    ].sort_values("threadCount")

    if data.empty:
        print(f"Nincs adat ehhez: {method} {algorithm}, n = {n_value}")
        continue

    plt.figure(figsize=(9, 5))

    plt.plot(
        data["threadCount"],
        data["time"],
        marker="o"
    )

    title_n = f"{n_value:,}".replace(",", " ")

    plt.title(
        f"{method} {algorithm}: futási idő szálak/folyamatok szerint\n"
        f"n = {title_n}"
    )
    plt.xlabel("Szálak / folyamatok száma")
    plt.ylabel("Futási idő (másodperc)")
    plt.xticks(list(data["threadCount"]))
    plt.grid(True, alpha=0.35)

    created_files.append(save_chart(filename))


speedup_plots = [
    ("mergesort", 1_000_000_000, "grafikon_14.png"),
    ("quicksort", 1_000_000_000, "grafikon_15.png"),
    ("oddeven", 100_000, "grafikon_16.png"),
]

for algorithm, n_value, filename in speedup_plots:
    sequential_data = df[
        (df["method"] == "sequential") &
        (df["algorithm"] == algorithm) &
        (df["n"] == n_value)
    ]

    if sequential_data.empty:
        print(f"Nincs sequential adat ehhez: {algorithm}, n = {n_value}")
        continue

    sequential_time = sequential_data["time"].min()

    data = df[
        (df["method"] != "sequential") &
        (df["algorithm"] == algorithm) &
        (df["n"] == n_value)
    ].copy()

    if data.empty:
        print(f"Nincs párhuzamos adat ehhez: {algorithm}, n = {n_value}")
        continue

    data["speedup"] = sequential_time / data["time"]

    plt.figure(figsize=(9, 5))

    for method in sorted(data["method"].unique()):
        method_data = data[data["method"] == method].sort_values("threadCount")

        plt.plot(
            method_data["threadCount"],
            method_data["speedup"],
            marker="o",
            label=method
        )

    title_n = f"{n_value:,}".replace(",", " ")

    plt.axhline(1.0, linestyle="--", linewidth=1)
    plt.title(
        f"{algorithm}: speedup a szekvenciális változathoz képest\n"
        f"n = {title_n}, speedup = szekvenciális idő / párhuzamos idő"
    )
    plt.xlabel("Szálak / folyamatok száma")
    plt.ylabel("Speedup, hányszoros gyorsulás")
    plt.legend()
    plt.grid(True, alpha=0.35)

    created_files.append(save_chart(filename))

