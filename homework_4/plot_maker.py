import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("task_21.csv")
# print(df["std::sort"])
fig, ax = plt.subplots(1, 1)
ax.plot([i*10 for i in range(len(df["std::sort"]))], df["std::sort"], label='std::sort')
ax.plot([i*10 for i in range(len(df["std::nth_element"]))], df["std::nth_element"], label="std::nth_element")
ax.set_xlabel("num_elements")
ax.set_ylabel("num_comparisons")
plt.legend()
plt.savefig("plot.pdf")
