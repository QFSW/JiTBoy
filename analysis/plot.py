import os
import matplotlib.pyplot as plt
import matplotlib.style as style
import numpy as np

style.use('ggplot')
bar_padding = 0.1

def savefig(path):
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)
    
    plt.tight_layout()
    plt.savefig(path)
    plt.clf()

def header_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Execution Time'
    }

    if name in mapping:
        return mapping[name]

    return name.title()

def axis_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Time (μs)'
    }

    if name in mapping:
        return mapping[name]

    return name.title()

def scatter(datasets, x, y, path, xscale='linear', yscale='linear'):
    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))
        ydata = list(map(lambda i: i[y], data))

        plt.plot(xdata, ydata, 'x', label=name)

    plt.xlabel(axis_case(x))
    plt.ylabel(axis_case(y))
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.legend(loc='upper left')
    plt.title("%s vs %s" % (header_case(y), header_case(x)))
    savefig(path)

def bar(data, x, y, path, xscale='linear', yscale='linear'):
    xdata = list(map(lambda i: i[x], data))
    ydata = list(map(lambda i: i[y], data))

    plt.xlabel(axis_case(x))
    plt.ylabel(axis_case(y))
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.title("%s vs %s" % (header_case(y), header_case(x)))
    plt.bar(xdata, ydata)
    savefig(path)

def bar_categoric(data, x, y, path, yscale='linear'):
    xdata = list(map(lambda i: i[x], data))
    ydata = list(map(lambda i: i[y], data))
    ypos = np.arange(len(xdata))

    plt.ylabel(axis_case(y))
    plt.yscale(yscale)
    plt.xticks(ypos, xdata, rotation=90)
    plt.title("%s" % header_case(y))
    plt.bar(ypos, ydata, align='center', label='JIT')
    plt.legend(loc='upper left')
    savefig(path)

def histogram(data, x, path, bins=30, yscale='linear'):
    xdata = list(map(lambda i: i[x], data))

    plt.xlabel(axis_case(x))
    plt.ylabel("Count")
    plt.yscale(yscale)
    plt.title(header_case(x))
    plt.hist(xdata, bins=bins)
    savefig(path)