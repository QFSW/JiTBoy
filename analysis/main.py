import loader
import plot
import data_proc
import re

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
        'source instructions emulated',
        'mips',
        'hotness',
        'host block size',
        'source block size',
        'compilation inefficiency',
        'execution inefficiency'
    ]

    for p in plots:
        plot.histogram(data, p, '%s/%s.png' % (base_hist, p))

    for p in plots:
        plot.boxplot(data, p, '%s/%s.png' % (base_box, p))

def draw_testbatches(data):
    plots = [
        ('unroll'   , 'unroll\([0-9]+\/[0-9]+\)'),
        ('fibonacci', 'fibonacci\([0-9]+\)')
    ]

    for (name, pattern) in plots:
        base = "%s/%s" % (output_base, name)
        r = re.compile(pattern)
        processed = data_proc.filter_rows(data, lambda x: r.match(x['name']))

        plot.bar_categoric(processed, 'name', 'time', '%s/time.png' % base, yscale='log')
        plot.bar_categoric(processed, 'name', 'mips', '%s/mips.png' % base, yscale='log')

def draw_jit_vs_interpreter(data):
    plots = [
        ('time', 'Execution Time (μs)'),
        ('mips', 'mips')
    ]

    for p in plots:
        values = []
        jit = data['JIT']
        interpreter = data['Interpreter']

        for i in range(len(jit)):
            name = jit[i]['name']
            items = list(filter(lambda i: i['name'] == name, interpreter))

            if len(items) > 0:
                row = {
                    'jit'          : jit[i][p[0]],
                    'interpreter'  : items[0][p[0]]
                }
                values.append(row)
        
        processed = {p[1]: values}

        base = "%s/scatter" % output_base
        plot.scatter(processed, 'jit', 'interpreter', '%s/%s.png' % (base, p[0]), xscale='log', yscale='log', line=True)

def main():
    data = {
        'JIT': loader.load_data('output/results_jit.csv', jit=True),
        'Interpreter': loader.load_data('output/results_interpreter.csv')
    }

    draw_jit_vs_interpreter(data)
    draw_scatters(data)
    draw_histograms(data)
    draw_testbatches(data)

if __name__ == "__main__":
    main()