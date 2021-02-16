import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

df = pd.read_csv('statistics/table.csv')
# df.to_html('statistics/table.html')

fig, ax =plt.subplots(figsize=(4,3))
ax.axis('tight')
ax.axis('off')
the_table = ax.table(cellText=df.values, colLabels=df.columns, loc='center', cellLoc='center')
# the_table.set_fontsize(14)
pp = PdfPages("statistics/table.pdf")
pp.savefig(fig, bbox_inches='tight')
pp.close()
