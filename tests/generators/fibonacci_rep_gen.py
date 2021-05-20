import os
import shutil

tpl_head_path = "tests/generators/fibonacci_rep_head_tpl.s"
tpl_seg_path = "tests/generators/fibonacci_rep_seg_tpl.s"
path = "tests/mips/perf/recursion/fibonacci_rep/"
reps = [10, 100]
tests = 20

def fib(n):
    if (n <= 1):
        return n
    return fib(n - 1) + fib(n - 2)

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    tpl_head = ""
    with open(tpl_head_path, 'r') as f:
        tpl_head = f.read()

    tpl_seg = ""
    with open(tpl_seg_path, 'r') as f:
        tpl_seg = f.read()

    for k in reps:
        for i in range(tests):
            with open("%sfibonacci_rep_%03d_%02d.s" % (path, k, i), 'w') as f:
                test_input = str(i)
                test_output = str(fib(i) * k)
                test_num = str(k)

                test = tpl_head
                test += '\n\n'

                for j in range(k):
                    test += 'addi $6, $10, 0\n'
                    test += 'jal fibonacci_%d\n' % j
                    test += 'nop\n'
                    test += 'add $11, $11, $5\n'
                    test += '\n'

                test += 'j end\n'

                for j in range(k):
                    test += '\n'
                    test += tpl_seg.replace('$NUM', str(j))
                    test += '\n'

                test += 'end:'

                test = test.replace('$INPUT', test_input)
                test = test.replace('$OUTPUT', test_output)
                test = test.replace('$NUM', test_num)
                f.write(test)

if __name__ == "__main__":
    main()