import matplotlib.pyplot as plt
from loader import load_data

data_path = "output/results.csv"

def plot(data, x, y, path, xscale='linear', yscale='linear'):
    xdata = list(map(lambda i: i[x], data))
    ydata = list(map(lambda i: i[y], data))

    plt.xlabel(x)
    plt.ylabel(y)
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.plot(xdata, ydata, 'o', color='black')
    plt.savefig(path)
    plt.clf()

def main():
    data = load_data(data_path)
    plot(data, 'hotness', 'mips', 'output/graph1.png', xscale='log', yscale='log')
    plot(data, 'source block size', 'efficiency c', 'output/graph2.png', xscale='log', yscale='linear')

if __name__ == "__main__":
    main()