import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib import ticker

PATH = "statistics/results"

df = pd.read_csv(PATH+'.csv', index_col=0).round(2)
timings = [10**i for i in range(2, 6)]

fig, ax =plt.subplots(figsize=(4,3))
ax.axis('tight')
ax.axis('off')
the_table = ax.table(cellText=df.values, colLabels=df.columns, rowLabels=df.index, loc='center', cellLoc='center')
pp = PdfPages("statistics/results.pdf")
pp.savefig(fig, bbox_inches='tight')
pp.close()

fig, ax =plt.subplots(figsize=(8, 6))
ax.plot(timings, df.loc['array (μs)'], label='std::array')
ax.plot(timings, df.loc['vector (μs)'], label='std::vector')
ax.plot(timings, df.loc['set (μs)'], label='std::set')
ax.set_yscale("log")
ax.set_xscale("log")
ax.get_xaxis().set_major_formatter(ticker.ScalarFormatter())
ax.grid()
ax.set_xlabel("log size")
ax.set_ylabel("log time")

plt.legend(loc='best')
plt.savefig(PATH+'_plot.pdf')
# print(df.loc['array (μs)'])
