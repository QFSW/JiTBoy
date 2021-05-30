import loader
import plot
import data_proc
import re

data_path = "output/results.csv"
output_base = "output/graphs"

def draw_scatters(data):
    base = "%s/scatter" % output_base

    data_emulators = data_proc.select(data, ['JIT -L', 'Interpreter', 'Hybrid -L'])
    plot.scatter(data_emulators, 'hotness', 'mips', '%s/hotness.png' % base, xscale='log', yscale='log')

    data_jit = data_proc.select(data, ['JIT'])
    plot.scatter(data_jit, 'source block size', 'compilation inefficiency', '%s/c-efficiency.png' % base, xscale='log', yscale='linear')
    plot.scatter(data_jit, 'source block size', 'execution inefficiency', '%s/e-efficiency.png' % base, xscale='log', yscale='linear')

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

def draw_testbatches(group_name, data, col_map=None):
    plots = [
        ('unroll'             , 'unroll\([0-9]+\/[0-9]+\)'),
        ('fibonacci'          , 'fibonacci\([0-9]+\)'),
        ('fibonacci_rep[10]'  , 'fibonacci_rep\[10\]\([0-9]+\)'),
        ('fibonacci_rep[100]' , 'fibonacci_rep\[100\]\([0-9]+\)'),
        ('factorial'          , 'factorial\([0-9]+\)'),
        ('primal'             , 'primal\([0-9]+\)'),
        ('memcpy'             , 'memcpy\([0-9]+\)'),
        ('memcpyw'            , 'memcpyw\([0-9]+\)')
    ]

    for (name, pattern) in plots:
        base = "%s/tests/%s/%s" % (output_base, group_name, name)
        r = re.compile(pattern)
        processed = data_proc.filter_rows(data, lambda x: r.match(x['name']))

        plot.line_categoric(processed, 'name', 'time', '%s: Execution Time' % name, '%s/time.png' % base, yscale='log', col_map=col_map)
        plot.line_categoric(processed, 'name', 'mips', '%s: Performance' % name, '%s/mips.png' % base, yscale='linear', col_map=col_map)

def draw_x_vs_y(data, xname, yname):
    plots = [
        ('time', 'Execution Time (μs)'),
        ('mips', 'mips')
    ]

    for p in plots:
        values = []
        x = data[xname]
        y = data[yname]

        for i in range(len(x)):
            name = x[i]['name']
            items = list(filter(lambda i: i['name'] == name, y))

            if len(items) > 0:
                row = {
                    xname: x[i][p[0]],
                    yname: items[0][p[0]]
                }
                values.append(row)
        
        processed = {p[1]: values}

        base = "%s/scatter/%s-vs-%s-" % (output_base, xname, yname)
        plot.scatter(processed, xname, yname, '%s%s.png' % (base, p[0]), xscale='log', yscale='log', line=True)

def draw_vs_scatters(data):
    draw_x_vs_y(data, 'JIT', 'Interpreter')
    draw_x_vs_y(data, 'Hybrid', 'Interpreter')
    draw_x_vs_y(data, 'Hybrid', 'JIT')
    draw_x_vs_y(data, 'JIT -L', 'JIT')
    draw_x_vs_y(data, 'JIT -L', 'Interpreter')
    draw_x_vs_y(data, 'JIT -L', 'Hybrid')

def main():
    data = {
        'JIT'           : loader.load_data('output/results_jit.csv', jit=True),
        'JIT -L'        : loader.load_data('output/results_jit(-L).csv'),
        'Interpreter'   : loader.load_data('output/results_interpreter.csv'),
        'Hybrid'        : loader.load_data('output/results_hybrid.csv'),
        'Hybrid -L'     : loader.load_data('output/results_hybrid(-L).csv'),
        'Hybrid -LS'    : loader.load_data('output/results_hybrid(-LS).csv'),
    }

    col_map = plot.make_col_map(data)
    data_emulators = data_proc.select(data, ['JIT -L', 'Interpreter', 'Hybrid -L'])
    data_jit       = data_proc.select(data, ['JIT', 'JIT -L'])
    data_hybrid    = data_proc.select(data, ['Hybrid', 'Hybrid -L', 'Hybrid -LS'])

    draw_vs_scatters(data)
    draw_scatters(data)
    draw_histograms(data)

    draw_testbatches('all', data, col_map)
    draw_testbatches('emulators', data_emulators, col_map)
    draw_testbatches('jit', data_jit, col_map)
    draw_testbatches('hybrid', data_hybrid, col_map)

if __name__ == "__main__":
    main()