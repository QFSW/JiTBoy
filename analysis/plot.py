import matplotlib.pyplot as plt
import matplotlib.style as style

style.use('ggplot')

def scatter(data, x, y, path, xscale='linear', yscale='linear'):
    xdata = list(map(lambda i: i[x], data))
    ydata = list(map(lambda i: i[y], data))

    plt.xlabel(x)
    plt.ylabel(y)
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.plot(xdata, ydata, 'x', color='black')
    plt.savefig(path)
    plt.clf()