import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib import ticker

df = pd.read_csv("statistics/table.csv");
x = df['number_of_elements'].values
y = df['number_of_collisions'].values
data = pd.read_csv('statistics/hash_occupancy.csv', header=None).values[0]
fig, ax = plt.subplots(1, 3, figsize=(18, 5), gridspec_kw={'width_ratios': [1, 1, 2]})
ax[0].plot(x, y)
ax[0].set_xlabel("number_of_instances")
ax[0].set_ylabel("number_of_collisions")
ax[0].grid()
ax[1].plot(x**2, y)
ax[1].set_xlabel("number_of_instances${}^2$")
ax[1].set_ylabel("number_of_collisions")
ax[1].grid()
ax[2].bar([i for i in range(len(data))], data, width=0.6)
ax[2].set_xlabel("number of instances")
ax[2].set_ylabel("bucket_index")
plt.savefig('statistics/plot.pdf')