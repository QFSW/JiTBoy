import os
import shutil

path = "tests/mips/perf/unroll/"
tests = 10

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    loop_size = 2 ** (tests - 1)
    for i in range(tests):
        with open("%sunroll_%d.s" % (path, i), 'w') as f:
            inner_loops = 2 ** i
            inner_loop_size = loop_size // inner_loops

            f.write('# name: unroll(%d/%d)\n' % (loop_size, inner_loops))
            f.write('# desc: loop of %d unrolled into chunks of %d\n' % (loop_size, inner_loop_size))
            f.write('#\n')
            f.write('# init: $1 = 0\n')
            f.write('# init: $2 = %d\n' % loop_size)
            f.write('# assert: $1 == %d\n' % loop_size)
            f.write('\n')

            if loop_size != inner_loop_size:
                f.write('start:\n')

            for j in range(inner_loop_size):
                f.write("addi $1, $1, 1\n")

            if loop_size != inner_loop_size:
                f.write('bne $1, $2, start\n')
                f.write('nop\n')

if __name__ == "__main__":
    main()