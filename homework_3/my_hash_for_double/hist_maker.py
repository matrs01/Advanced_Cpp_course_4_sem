import pandas as pd
import matplotlib.pyplot as plt

# data = pd.read_csv('statistics/stats.csv').columns.to_numpy()
data = pd.read_csv('statistics/stats.csv', header=None).values[0][:-1]
# print(data)
fig, ax =plt.subplots(figsize=(6,4))
plt.bar([i for i in range(len(data))], data, width=0.6)
ax.set_ylabel("number_of_elements_in_bucket")
ax.set_xlabel("bucket_index")
plt.savefig('statistics/stats.pdf')
