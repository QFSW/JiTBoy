import os
import shutil

path = "tests/mips/perf/ladder/"
tests = 12

def main():
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    for i in range(tests):
        for k in range(tests):
            with open("%sladder_%03d.s" % (path, i * tests + k), 'w') as f:
                blocks = 2 ** i
                loops = 2 ** k
                output = loops * blocks

                f.write('# name: ladder(%d*%d)\n' % (blocks, loops))
                f.write('# desc: ladder of %d blocks, looped %d times\n' % (blocks, loops))
                f.write('#\n')
                f.write('# init: $1 = 0\n')
                f.write('# init: $2 = 0\n')
                f.write('# init: $3 = %d\n' % loops)
                f.write('# assert: $1 == %d\n' % output)
                f.write('\n')

                for j in range(blocks):
                    f.write('block_%d:\n' % j)
                    f.write('addi $1, $1, 1\n')
                    if j != blocks - 1:
                        f.write('j block_%d\n' % (j + 1))
                        f.write('nop\n')
                    f.write('\n')

                f.write('addi $2, $2, 1\n')
                f.write('bne $2, $3, block_0\n')
                f.write('nop\n')

if __name__ == "__main__":
    main()