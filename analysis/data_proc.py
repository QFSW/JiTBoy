def select(data, names):
    result = {}
    for name in data:
        if name in names:
            result[name] = data[name]
    
    return result

def filter_rows(data, f):
    result = {}
    for name in data:
        entries = list(filter(f, data[name]))
        result[name] = entries
    
    return result