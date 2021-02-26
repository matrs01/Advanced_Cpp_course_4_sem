import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

collision_stat = pd.read_csv("statistics/collision_stat.csv", index_col=0)

# print(collision_stat.iloc[0 ].values)

fig, ax = plt.subplots(1, 2, figsize=(12, 5))
for i in range(9):
    if not (i == 3 or i == 2 or i == 7):
        ax[1].plot(collision_stat.columns.values.astype(int), collision_stat.iloc[i].values,
                   label=collision_stat.index[i], linewidth=1)
    ax[0].plot(collision_stat.columns.values.astype(int), collision_stat.iloc[i].values,
                  label=collision_stat.index[i], linewidth=1)

ax[0].set_xlabel(f"number_of_instances")
ax[0].set_ylabel("number_of_collisions")
ax[0].legend(loc="best")
ax[0].title.set_text("All hash-functions")
ax[1].set_xlabel(f"number_of_instances")
ax[1].set_ylabel("number_of_collisions")
ax[1].legend(loc="best")
ax[1].title.set_text("Hash-functions without PJWHash, ELFHash and DEKHash")
plt.savefig("statistics/plot.pdf")