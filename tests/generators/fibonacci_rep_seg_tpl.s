# $5 is output, $6 is input
fibonacci_$NUM:
    fib_init_$NUM:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_$NUM
        nop

    fib_base_$NUM:
        addiu $5, $6, 0
        j fib_return_$NUM
        nop

    fib_rec_$NUM:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_$NUM
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_$NUM
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_$NUM:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop