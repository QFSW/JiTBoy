# name: fibonacci_rep[10](5)
# desc: recursive fibonacci using stack, computes fib(5). fibonacci code repeated 10 times
#
# init: $sp = 0x20000000
# init: $10 = 5
# assert: $11 == 50

addi $6, $10, 0
jal fibonacci_0
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_1
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_2
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_3
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_4
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_5
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_6
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_7
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_8
nop
add $11, $11, $5

addi $6, $10, 0
jal fibonacci_9
nop
add $11, $11, $5

j end

# $5 is output, $6 is input
fibonacci_0:
    fib_init_0:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_0
        nop

    fib_base_0:
        addiu $5, $6, 0
        j fib_return_0
        nop

    fib_rec_0:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_0
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_0
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_0:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_1:
    fib_init_1:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_1
        nop

    fib_base_1:
        addiu $5, $6, 0
        j fib_return_1
        nop

    fib_rec_1:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_1
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_1
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_1:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_2:
    fib_init_2:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_2
        nop

    fib_base_2:
        addiu $5, $6, 0
        j fib_return_2
        nop

    fib_rec_2:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_2
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_2
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_2:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_3:
    fib_init_3:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_3
        nop

    fib_base_3:
        addiu $5, $6, 0
        j fib_return_3
        nop

    fib_rec_3:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_3
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_3
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_3:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_4:
    fib_init_4:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_4
        nop

    fib_base_4:
        addiu $5, $6, 0
        j fib_return_4
        nop

    fib_rec_4:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_4
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_4
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_4:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_5:
    fib_init_5:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_5
        nop

    fib_base_5:
        addiu $5, $6, 0
        j fib_return_5
        nop

    fib_rec_5:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_5
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_5
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_5:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_6:
    fib_init_6:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_6
        nop

    fib_base_6:
        addiu $5, $6, 0
        j fib_return_6
        nop

    fib_rec_6:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_6
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_6
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_6:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_7:
    fib_init_7:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_7
        nop

    fib_base_7:
        addiu $5, $6, 0
        j fib_return_7
        nop

    fib_rec_7:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_7
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_7
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_7:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_8:
    fib_init_8:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_8
        nop

    fib_base_8:
        addiu $5, $6, 0
        j fib_return_8
        nop

    fib_rec_8:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_8
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_8
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_8:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

# $5 is output, $6 is input
fibonacci_9:
    fib_init_9:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        addi $1, $6, -1
        bgtz $1, fib_rec_9
        nop

    fib_base_9:
        addiu $5, $6, 0
        j fib_return_9
        nop

    fib_rec_9:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        # fib(n - 1)
        addiu $6, $6, -1
        jal fibonacci_9
        nop

        # store fib(n - 1)
        lw $6, 0($sp)
        sw $5, 0($sp)

        # fib(n - 2)
        addiu $6, $6, -2
        jal fibonacci_9
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        # fib(n - 1) + fib(n - 2)
        add $5, $5, $6

    fib_return_9:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop
end: