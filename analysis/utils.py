def union(*args):
    acc = []
    for l in args:
        for x in l:
            if x not in acc:
                acc.append(x)
    return acc