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
    'source instructions simulated'
]

def parse_attributes(raw):
    for col in rename_cols:
        for i in range(len(raw)):
            if raw[i] == col[0]:
                raw[i] = col[1]
    
    return raw

def load_data(path):
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
                    item['mips'] = item['source instructions simulated'] / item['time']
                    item['hotness'] = item['blocks executed'] / item['blocks']
                    item['host block size'] = item['host instructions'] / item['blocks']
                    item['source block size'] = item['source instructions'] / item['blocks']
                    item['compilation inefficiency'] = item['host instructions'] / item['source instructions']
                    item['execution inefficiency'] = item['host instructions executed'] / item['source instructions simulated']
                    data.append(item)
                line_count += 1

    return data