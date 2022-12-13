import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from IPython import embed

waypoints = np.loadtxt("segment1.txt", delimiter=" ")
waypoints = waypoints[waypoints[:, -1] == 200]
waypoints = waypoints[:-1]

plt.subplot(221, projection='3d')
plt.plot(*waypoints[:, :3].T)
plt.show()

embed()