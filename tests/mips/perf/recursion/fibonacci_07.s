# name: fibonacci(7)
# desc: recursive fibonacci using stack, computes fib(7)
#
# init: $sp = 0x20000000
# init: $6 = 7
# assert: $5 == 13

setup:
    jal fibonacci
    nop

    j end


# $5 is output, $6 is input
fibonacci:
    fib_init:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec
        nop

    fib_base:
        addiu $5, $6, 0
        j fib_return
        nop

    fib_rec:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

end: