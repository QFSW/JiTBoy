import os
import matplotlib.pyplot as plt
import matplotlib.style as style
import matplotlib.ticker as mticker
import numpy as np
import data_proc
import utils

style.use('ggplot')

bar_padding = 0.1

def savefig(path):
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)

    def log_format(y, pos):
        if y == 0:
            return '0'

        dp = int(np.maximum(-np.log10(abs(y)), 0))
        fstr = '{{:.{:1d}f}}'.format(dp)
        return fstr.format(y)

    for axis in plt.gcf().axes:
        for ax in [axis.xaxis, axis.yaxis]:
            if ax.get_scale() == 'log':
                ax.set_major_formatter(mticker.FuncFormatter(log_format))
    
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
    title = "%s vs %s" % (header_case(y), header_case(x))
    print("Drawing scatterplot %s" % title)

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))
        ydata = list(map(lambda i: i[y], data))

        plt.plot(xdata, ydata, 'x', alpha=0.8, label=name)

    plt.xlabel(axis_case(x))
    plt.ylabel(axis_case(y))
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.legend(loc='upper left')
    plt.title(title)
    savefig(path)

def bar_categoric(datasets, x, y, path, yscale='linear'):
    title = header_case(y)
    print("Drawing bar chart %s" % title)

    xdatas = []
    for name in datasets:
        xdata = list(map(lambda i: i[x], datasets[name]))
        xdatas.append(xdata)

    xdata = utils.union(*xdatas)
    ypos = np.arange(len(xdata))

    cols = len(datasets)
    width = (1 - 2 * bar_padding) / cols

    for i, name in enumerate(datasets):
        data = datasets[name]

        ydata = []
        for j in range(len(xdata)):
            xitem = xdata[j]
            items = list(filter(lambda i: i[x] == xitem, data))
            if len(items) > 0:
                ydata.append(items[0][y])
            else:
                ydata.append(0)

        offset = i - cols / 2
        plt.bar(ypos + offset * width, ydata, width=width, align='center', label=name)

    plt.ylabel(axis_case(y))
    plt.yscale(yscale)
    plt.xticks(ypos, xdata, rotation=90)
    plt.title(title)
    plt.legend(loc='upper left')
    savefig(path)

def histogram(datasets, x, path, bins=30, yscale='linear'):
    title = header_case(x)
    print("Drawing histogram %s" % title)

    cols = len(datasets)
    alpha = 1 / cols

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))

        plt.hist(xdata, density=True, bins=bins, label=name, alpha=alpha)

    plt.xlabel(axis_case(x))
    plt.ylabel("%")
    plt.yscale(yscale)
    plt.title(title)
    plt.legend(loc='upper left')
    savefig(path)

def boxplot(datasets, x, path):
    title = header_case(x)
    print("Drawing box plot %s" % title)

    datas = []
    names = []

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))

        datas.append(xdata)
        names.append(name)

    plt.boxplot(datas, showfliers=False, labels=names)
    plt.ylabel(axis_case(x))
    plt.title(title)
    savefig(path)