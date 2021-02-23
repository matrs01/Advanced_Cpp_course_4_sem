import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

collision_stat = pd.read_csv("statistics/collisions_stat.csv")
occupancy_stat = pd.read_csv("statistics/occupancy_stat.csv", header=None).values
# print(collision_stat.iloc[0].index)
fig, ax = plt.subplots(2, 2, figsize=(18, 18))
for i in range(9):
    ax[0, 0].plot(np.array(collision_stat.iloc[i].index[1:], dtype=float), collision_stat.iloc[i].values[1:],
                  label=collision_stat.iloc[i].values[0], linewidth=1)
ax[0, 0].set_xlabel(f"load_factor (1024 buckets)")
ax[0, 0].set_ylabel("number_of_collisions")
ax[0, 0].legend(loc="best")

ax[0, 1].bar([collision_stat.iloc[i].values[0] for i in range(9)],
             [collision_stat.iloc[i].values[-1] - 0 for i in range(9)],
             bottom=0)
ax[0, 1].set_ylabel("number_of_collisions")
ax[0, 1].title.set_text(f"For load_factor = {collision_stat.iloc[0].index[-1]}")

ax[1, 0].bar([collision_stat.iloc[i].values[0] for i in range(9)],
             [collision_stat.iloc[i].values[10] - 0 for i in range(9)],
             bottom=0)
ax[1, 0].set_ylabel("number_of_collisions")
ax[1, 0].title.set_text(f"For load_factor = {collision_stat.iloc[0].index[10]}")

ax[1, 1].bar([collision_stat.iloc[i].values[0] for i in range(9)],
             [collision_stat.iloc[i].values[4] - 0 for i in range(9)],
             bottom=0)
ax[1, 1].set_ylabel("number_of_collisions")
ax[1, 1].title.set_text(f"For load_factor = {collision_stat.iloc[0].index[4]}")

ax[0, 1].tick_params(axis='x', rotation=45)
ax[1, 1].tick_params(axis='x', rotation=45)
ax[1, 0].tick_params(axis='x', rotation=45)
plt.savefig("statistics/collisions_stat.pdf")

fig, ax = plt.subplots(3, 3, figsize=(18, 18))
for i in range(3):
    for j in range(3):
        ax[i, j].bar([i for i in range(len(occupancy_stat[i+j][1:]))], occupancy_stat[i+j][1:], width=0.6)
        ax[i, j].title.set_text(occupancy_stat[i+j][0])
plt.savefig("statistics/occupancy_stat.pdf")
