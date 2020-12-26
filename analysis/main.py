import loader
import plot
import data_proc

data_path = "output/results.csv"
output_base = "output/graphs"

def draw_scatters(data):
    base = "%s/scatter" % output_base
    plot.scatter(data, 'hotness', 'mips', '%s/hotness.png' % base, xscale='log', yscale='log')
    plot.scatter(data, 'source block size', 'compilation inefficiency', '%s/c-efficiency.png' % base, xscale='log', yscale='linear')
    plot.scatter(data, 'source block size', 'execution inefficiency', '%s/e-efficiency.png' % base, xscale='log', yscale='linear')

def draw_histograms(data):
    base_hist = "%s/histogram" % output_base
    base_box = "%s/boxplot" % output_base
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
        'source block size',
        'compilation inefficiency',
        'execution inefficiency'
    ]
    for p in plots:
        plot.histogram(data, p, '%s/%s.png' % (base_hist, p), yscale="log")
        plot.boxplot(data, p, '%s/%s.png' % (base_box, p))

def draw_unroll(data):
    import re
    base = "%s/unroll" % output_base
    pattern = re.compile('unroll\([0-9]+\/[0-9]+\)')
    data = data_proc.select(data, ['JIT', 'JIT (Release)'])
    unroll = data_proc.filter_rows(data, lambda x: pattern.match(x['name']))

    plot.bar_categoric(unroll, 'name', 'time', '%s/time.png' % base)
    plot.bar_categoric(unroll, 'name', 'mips', '%s/mips.png' % base)

def main():
    data = {
        'JIT': loader.load_data('output/results.csv')
    }

    draw_scatters(data)
    draw_histograms(data)
    draw_unroll(data)

if __name__ == "__main__":
    main()