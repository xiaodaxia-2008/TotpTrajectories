import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from IPython import embed

data = np.loadtxt("out.txt", skiprows=1)
timestamps = data[:, 0]
positions = data[:, 1:]

plt.subplot(211, projection='3d')
plt.plot(*positions[:, :3].T)
plt.subplot(212)
plt.plot(timestamps, positions)
plt.legend(list("xyzca"))
plt.title("positions")
plt.show()

embed()