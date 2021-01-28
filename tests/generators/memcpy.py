import os
import shutil

tpl_path = "tests/generators/memcpy_tpl.s"
path = "tests/mips/perf/iteration/memcpy/"
tests = 20

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    tpl = ""
    with open(tpl_path, 'r') as f:
        tpl = f.read()

    for i in range(tests):
        with open("%smemcpy_%02d.s" % (path, i), 'w') as f:
            val = 2 ** i
            test_input = str(val)
            test = tpl
            test = test.replace('$INPUT', test_input)
            f.write(test)

if __name__ == "__main__":
    main()