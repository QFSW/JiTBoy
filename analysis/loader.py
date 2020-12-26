import csv

rename_cols = [
    ('time (us)', 'time')
]

int_cols = [
    'time',
    'blocks',
    'blocks executed',
    'host instructions',
    'source instructions',
    'host instructions executed',
    'source instructions emulated'
]

def parse_attributes(raw):
    for col in rename_cols:
        for i in range(len(raw)):
            if raw[i] == col[0]:
                raw[i] = col[1]
    
    return raw

statistics = {
#   'metric'                        : (jit  , 'op', 'operand1'                    , 'operand2'),
    'blocks'                        : (True , '=>'),
    'blocks executed'               : (True , '=>'),
    'host instructions'             : (True , '=>'),
    'source instructions'           : (True , '=>'),
    'host instructions executed'    : (True , '=>'),

    'mips'                          : (False, '/' , 'source instructions emulated', 'time'),
    'hotness'                       : (True , '/' , 'blocks executed'             , 'blocks'),
    'host block size'               : (True , '/' , 'host instructions'           , 'blocks'),
    'source block size'             : (True , '/' , 'source instructions'         , 'blocks'),
    'compilation inefficiency'      : (True , '/' , 'host instructions'           , 'source instructions'),
    'execution inefficiency'        : (True , '/' , 'host instructions executed'  , 'source instructions emulated')
}

statistic_cache = {}

def compute_statistics(item, jit=False):
    name = item['name']
    if name not in statistic_cache:
        statistic_cache[name] = {}

    for metric in statistics:
        jit_only = statistics[metric][0]
        operator = statistics[metric][1]

        if jit or not jit_only:
            if operator == '=>':
                pass
            elif operator == '/':
                left = statistics[metric][2]
                right = statistics[metric][3]
                item[metric] = item[left] / item[right]
            else:
                raise ValueError("Unsupported operator %s" % operator)

            if metric not in statistic_cache[name]:
                statistic_cache[name][metric] = item[metric]
        else:
            item[metric] = statistic_cache[name][metric]

def load_data(path, jit=False):
    print("Loading data %s" % path)

    data = []
    attributes = []

    with open(path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',', skipinitialspace=True)
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                attributes = parse_attributes(row)
                line_count += 1
            else:
                item = {}
                for i, cell in enumerate(row):
                    item[attributes[i]] = cell

                for col in int_cols:
                    item[col] = int(item[col])
                
                if item['status'] == 'passed':
                    compute_statistics(item, jit)
                    data.append(item)

    return data