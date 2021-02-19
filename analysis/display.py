def header_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Execution Time',
        'jit' : 'JIT',
        'JIT' : 'JIT',
    }

    if name in mapping:
        return mapping[name]

    return name.title()

def axis_case(name):
    mapping = {
        'mips': 'mips',
        'time': 'Time (μs)',
        'jit' : 'JIT',
        'JIT' : 'JIT',
    }

    if name in mapping:
        return mapping[name]

    return name.title()