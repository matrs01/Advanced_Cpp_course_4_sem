import numpy as np
import matplotlib.pyplot as plt
from matplotlib import ticker

PATH = "graphics/plot"

array_1 = np.genfromtxt(PATH+"_1.csv", delimiter=',')
array_2 = np.genfromtxt(PATH+"_2.csv", delimiter=',')
array_3 = np.genfromtxt(PATH+"_3.csv", delimiter=',')
array_4 = np.genfromtxt(PATH+"_4.csv", delimiter=',')

fig, ax = plt.subplots(3, 2, figsize=(14, 14))
ax[0, 0].plot(array_1)
ax[0, 0].grid()
ax[0, 0].set_xlabel("number of pushbacks")
ax[0, 0].set_ylabel("capacity")

ax[1, 0].plot(array_2)
ax[1, 0].grid()
ax[1, 0].set_xlabel("number of reallocations")
ax[1, 0].set_ylabel("capacity")

ax[1, 1].plot(array_2)
ax[1, 1].grid()
ax[1, 1].set_yscale("log")
ax[1, 1].set_xlabel("number of reallocations")
ax[1, 1].set_ylabel("log capacity")
ax[1, 1].get_xaxis().set_major_formatter(ticker.ScalarFormatter())

ax[0, 1].plot(array_3)
ax[0, 1].grid()
ax[0, 1].set_xlabel("number of reallocations")
ax[0, 1].set_ylabel("capacity multiplicator")

ax[2, 0].plot([10**i for i in range(len(array_4)//2)], array_4[:len(array_4)//2])
ax[2, 0].grid()
ax[2, 0].set_xlabel("reserve argument")
ax[2, 0].set_ylabel("capacity")

ax[2, 1].plot([10**i for i in range(len(array_4)//2+1)][::-1], array_4[len(array_4)//2:])
ax[2, 1].grid()
ax[2, 1].set_xlabel("reserve argument")
ax[2, 1].set_ylabel("capacity")

plt.savefig(PATH+'.pdf')
