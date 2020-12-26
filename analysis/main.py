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
    plot.histogram(data, 'time (us)', '%s/time.png' % base, yscale="log")
    plot.histogram(data, 'mips', '%s/mips.png' % base, yscale="log")

def main():
    data = loader.load_data(data_path)
    draw_scatters(data)
    draw_histograms(data)

if __name__ == "__main__":
    main()