def header_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Execution Time',
        'jit' : 'JIT'
    }

    if name in mapping:
        return mapping[name]

    return name.title()

def axis_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Time (μs)',
        'jit' : 'JIT'
    }

    if name in mapping:
        return mapping[name]

    return name.title()