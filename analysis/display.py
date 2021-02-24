def header_case(name):
    mapping = {
        'mips'   : 'mips',
        'time'   : 'Execution Time',
        'jit'    : 'JIT',
        'JIT'    : 'JIT',
        'jit -L' : 'JIT -L',
        'JIT -L' : 'JIT -L',
    }

    if name in mapping:
        return mapping[name]

    return name.title()

def axis_case(name):
    mapping = {
        'mips'   : 'mips',
        'time'   : 'Time (μs)',
        'jit'    : 'JIT',
        'JIT'    : 'JIT',
        'jit -L' : 'JIT -L',
        'JIT -L' : 'JIT -L',
    }

    if name in mapping:
        return mapping[name]

    return name.title()