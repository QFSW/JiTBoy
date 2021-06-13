import os
import matplotlib.pyplot as plt
import matplotlib.style as style
import matplotlib.ticker as mticker
import numpy as np
import data_proc
import display
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

def scatter(datasets, x, y, path, xscale='linear', yscale='linear', line=False, col_map=None, sym_map=None):
    title = "%s vs %s" % (display.header_case(y), display.header_case(x))
    print("Drawing scatterplot %s" % title)

    if col_map is None:
        col_map = make_col_map(datasets)

    if sym_map is None:
        sym_map = make_sym_map(datasets)

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))
        ydata = list(map(lambda i: i[y], data))

        plt.plot(xdata, ydata, sym_map[name], alpha=0.75, label=name, color=col_map[name])

    plt.xlabel(display.axis_case(x))
    plt.ylabel(display.axis_case(y))
    plt.xscale(xscale)
    plt.yscale(yscale)
    plt.legend(loc='upper left')
    plt.title(title)

    if line:
        ax = plt.gcf().axes[0]
        lims = [
            np.min([ax.get_xlim(), ax.get_ylim()]),
            np.max([ax.get_xlim(), ax.get_ylim()]) 
        ]

        ax.plot(lims, lims, 'k-', alpha=0.5)
        ax.set_xlim(lims)
        ax.set_ylim(lims)

    savefig(path)

def bar_categoric(datasets, x, y, path, yscale='linear'):
    title = display.header_case(y)
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

        offset = i - (cols - 1) / 2
        plt.bar(ypos + offset * width, ydata, width=width, align='center', label=name)

    plt.ylabel(display.axis_case(y))
    plt.yscale(yscale)
    plt.xticks(ypos, xdata, rotation=90)
    plt.title(title)
    plt.legend(loc='upper left')
    savefig(path)

def line_categoric(datasets, x, y, title, path, yscale='linear', col_map=None):
    print("Drawing line chart %s" % title)

    if col_map is None:
        col_map = make_col_map(datasets)

    xdatas = []
    for name in datasets:
        xdata = list(map(lambda i: i[x], datasets[name]))
        xdatas.append(xdata)

    xdata = utils.union(*xdatas)
    ypos = np.arange(len(xdata))

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

        plt.plot(xdata, ydata, '-o', label=name, color=col_map[name])

    plt.ylabel(display.axis_case(y))
    plt.yscale(yscale)
    plt.xticks(ypos, xdata, rotation=90)
    plt.title(title)
    plt.legend(loc='upper left')
    savefig(path)

def histogram(datasets, x, path, bins=30, yscale='linear'):
    title = display.header_case(x)
    print("Drawing histogram %s" % title)

    cols = len(datasets)
    alpha = 1 / cols

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))

        plt.hist(xdata, density=True, bins=bins, label=name, alpha=alpha)

    plt.xlabel(display.axis_case(x))
    plt.ylabel("%")
    plt.yscale(yscale)
    plt.title(title)
    plt.legend(loc='upper left')
    savefig(path)

def boxplot(datasets, x, path, yscale='linear'):
    title = display.header_case(x)
    print("Drawing box plot %s" % title)

    datas = []
    names = []

    for name in datasets:
        data = datasets[name]
        xdata = list(map(lambda i: i[x], data))

        datas.append(xdata)
        names.append(name)

    plt.boxplot(datas, showfliers=False, labels=names)
    plt.yscale(yscale)
    plt.ylabel(display.axis_case(x))
    plt.title(title)
    savefig(path)

def make_col_map(data):
    i = 0
    col_map = {}
    cols = plt.rcParams['axes.prop_cycle'].by_key()['color']

    for name in data:
        col_map[name] = cols[i]
        i += 1

    return col_map

def make_sym_map(data):
    i = 0
    sym_map = {}
    syms = ['x', 'o', '^', 's', 'P', 'X']

    for name in data:
        sym_map[name] = syms[i % len(syms)]
        i += 1

    return sym_map