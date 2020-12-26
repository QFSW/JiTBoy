import loader
import plot

data_path = "output/results.csv"
output_base = "output/graphs"

def draw_scatters(data):
    base = "%s/scatter" % output_base
    plot.scatter(data, 'hotness', 'mips', '%s/hotness.png' % base, xscale='log', yscale='log')
    plot.scatter(data, 'source block size', 'efficiency c', '%s/efficiency.png' % base, xscale='log', yscale='linear')

def draw_histograms(data):
    base = "%s/histogram" % output_base
    plots = [
        'time',
        'blocks',
        'blocks executed',
        'host instructions',
        'source instructions',
        'host instructions executed',
        'source instructions simulated',
        'mips',
        'hotness',
        'host block size',
        'source block size'
    ]
    for p in plots:
        plot.histogram(data, p, '%s/%s.png' % (base, p), yscale="log")

def draw_unroll(data):
    import re
    base = "%s/unroll" % output_base
    pattern = re.compile('unroll\([0-9]+\/[0-9]+\)')
    unroll = list(filter(lambda x: pattern.match(x['name']), data))
    plot.bar_categoric(unroll, 'name', 'time', '%s/time.png' % base)
    plot.bar_categoric(unroll, 'name', 'mips', '%s/mips.png' % base)

def main():
    data = loader.load_data(data_path)
    draw_scatters(data)
    draw_histograms(data)
    draw_unroll(data)

if __name__ == "__main__":
    main()