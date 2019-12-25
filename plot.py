import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

data = np.loadtxt("hashmap.txt", delimiter=',', unpack=True)
xs, ys, zs = data

ax.scatter(xs, ys, zs)

ax.set_xlabel('X Board')
ax.set_ylabel('O Board')
ax.set_zlabel('Hash Key')

plt.show()
