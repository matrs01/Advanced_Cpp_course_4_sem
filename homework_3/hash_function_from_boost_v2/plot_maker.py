import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("statistics/collision_stat.csv");
x = df['Number_of_instances'].values
y = df['Number_of_collisions'].values
fig, ax = plt.subplots(1, 2, figsize=(12, 5))
ax[0].plot(x, y)
ax[0].set_xlabel("load_factor")
ax[0].set_ylabel("number_of_collisions")
ax[0].grid()
ax[1].plot(x**2, y)
ax[1].set_xlabel("load_factor${}^{2}$")
ax[1].set_ylabel("number_of_collisions")
ax[1].grid()
plt.savefig('statistics/plot.pdf')
