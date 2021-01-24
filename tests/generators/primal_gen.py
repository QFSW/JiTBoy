import os
import shutil

tpl_path = "tests/generators/primal_tpl.s"
path = "tests/mips/perf/iteration/primal/"
tests = 20

def primal(n):
    for i in range(2, n):
        if n % i == 0:
            return False
    return True

def find_prime(start):
    val = start
    while not primal(val):
        val += 1
    return val

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    tpl = ""
    with open(tpl_path, 'r') as f:
        tpl = f.read()

    for i in range(tests):
        with open("%sprimal_%02d.s" % (path, i), 'w') as f:
            val = find_prime(2 ** i)
            test_input = str(val)
            test_output = str(1 if primal(val) else 0)
            test = tpl
            test = test.replace('$INPUT', test_input)
            test = test.replace('$OUTPUT', test_output)
            f.write(test)

if __name__ == "__main__":
    main()