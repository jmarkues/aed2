import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 2 * np.pi)
ysen = np.sin(x)
ycos = np.cos(x)

xpontos = np.arange(0, 7)
ypontos = np.random.rand(len(xpontos))

sim = ypontos > ysen and ypontos < ycos

plt.plot(x, ysen)
plt.plot(x, ycos)
plt.scatter(xpontos[sim], ypontos[sim])
plt.legend(['sen', 'cos'])
plt.show()
