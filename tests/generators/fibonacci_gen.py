import os
import shutil

tpl_path = "tests/generators/fibonacci_tpl.s"
path = "tests/mips/perf/recursion/fibonacci/"
tests = 15

def fib(n):
    if (n <= 1):
        return n
    return fib(n - 1) + fib(n - 2)

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    tpl = ""
    with open(tpl_path, 'r') as f:
        tpl = f.read()

    for i in range(tests):
        with open("%sfibonacci_%02d.s" % (path, i), 'w') as f:
            test_input = str(i)
            test_output = str(fib(i))
            test = tpl
            test = test.replace('$INPUT', test_input)
            test = test.replace('$OUTPUT', test_output)
            f.write(test)

if __name__ == "__main__":
    main()