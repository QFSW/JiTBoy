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
        ('time', True),
        ('blocks', False),
        ('blocks executed', False),
        ('host instructions', False),
        ('source instructions', False),
        ('host instructions executed', False),
        ('source instructions emulated', False),
        ('mips', True),
        ('hotness', False),
        ('host block size', False),
        ('source block size', False),
        ('compilation inefficiency', False),
        ('execution inefficiency', False)
    ]

    for p in plots:
        name = p[0]
        processed = data
        if not p[1]:
            processed = {'JIT': data['JIT']}

        plot.histogram(processed, name, '%s/%s.png' % (base_hist, name))
        plot.boxplot(processed, name, '%s/%s.png' % (base_box, name))

def draw_testbatches(data):
    plots = [
        ('unroll'   , 'unroll\([0-9]+\/[0-9]+\)'),
        ('fibonacci', 'fibonacci\([0-9]+\)'),
        ('factorial', 'factorial\([0-9]+\)'),
        ('primal'   , 'primal\([0-9]+\)')
    ]

    for (name, pattern) in plots:
        base = "%s/tests/%s" % (output_base, name)
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