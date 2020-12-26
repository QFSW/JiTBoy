import os
import matplotlib.pyplot as plt
import matplotlib.style as style

style.use('ggplot')

def savefig(path):
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)

    plt.savefig(path)
    plt.clf()

def scatter(data, x, y, path, xscale='linear', yscale='linear'):
    xdata = list(map(lambda i: i[x], data))
    ydata = list(map(lambda i: i[y], data))

    plt.xlabel(x)
    plt.ylabel(y)
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.plot(xdata, ydata, 'x', color='black')
    savefig(path)

def histogram(data, x, path, bins=30, yscale='linear'):
    xdata = list(map(lambda i: i[x], data))

    plt.xlabel(x)
    plt.ylabel("Count")
    plt.yscale(yscale)
    plt.hist(xdata, bins=bins)
    savefig(path)