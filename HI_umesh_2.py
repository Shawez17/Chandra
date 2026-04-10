import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib import colors
from pathlib import Path

def plot_resistivity(csv_path, out_png, title, jig_pos, vmin=0.5, vmax=2.0, levels=50, figsize=(6,6)):
    data = np.loadtxt(csv_path, delimiter=",")
    rows, cols = data.shape
    x = np.arange(cols); y = np.arange(rows)
    X, Y = np.meshgrid(x, y)

    fig, ax = plt.subplots(figsize=figsize)
    levs = np.linspace(vmin, vmax, levels)
    contour = ax.contourf(X, Y, data, levels=levs, cmap='jet', norm=colors.Normalize(vmin=vmin, vmax=vmax))
    ax.set_aspect('equal')

    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.1)
    cbar = fig.colorbar(contour, cax=cax)
    cbar.set_ticks(np.arange(vmin, vmax+1e-6, 0.1))
    cbar.set_label("Resistivity (MΩ)")

    ax.set_title(title)

    # Corner annotations
    fig.text(0.02, 0.02, "1", fontsize=14, color="red",  ha="left", va="bottom")
    fig.text(0.98, 0.02, "2", fontsize=14, color="red", ha="right", va="bottom")
    fig.text(0.98, 0.98, "3", fontsize=14, color="red", ha="right", va="top")
    fig.text(0.02, 0.98, "4", fontsize=14, color="red", ha="left", va="top")

    fig.text(0.13, 0.05, f"JIG Position: {jig_pos}", fontsize=10)
    fig.text(0.13, 0.01, "1 corner: DAQ, 3 corner: HV", color="black", fontsize=10)

    plt.tight_layout()
    fig.savefig(out_png, dpi=300, bbox_inches="tight")
    plt.close(fig)

if __name__ == "__main__":
    plot_resistivity("SR_31_BH.csv", "RPC_31_BH_top.png", "Surface Resistivity_RPC-031_top", "H")
    plot_resistivity("SR_31_BI.csv", "RPC_31_BI_bot.png", "Surface Resistivity_RPC-031_bot", "I")

