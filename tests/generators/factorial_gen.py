import os
import shutil

tpl_path = "tests/generators/factorial_tpl.s"
path = "tests/mips/perf/recursion/factorial/"
tests = 12

def factorial(n):
    if (n <= 1):
        return 1
    return n * factorial(n - 1)

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    tpl = ""
    with open(tpl_path, 'r') as f:
        tpl = f.read()

    for i in range(tests):
        with open("%sfactorial_%02d.s" % (path, i), 'w') as f:
            test_input = str(i)
            test_output = str(factorial(i))
            test = tpl
            test = test.replace('$INPUT', test_input)
            test = test.replace('$OUTPUT', test_output)
            f.write(test)

if __name__ == "__main__":
    main()