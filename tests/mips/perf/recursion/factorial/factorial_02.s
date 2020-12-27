# name: factorial(2)
# desc: recursive factorial using stack, computes 2!
#
# init: $sp = 0x20000000
# init: $6 = 2
# assert: $5 == 2

setup:
    jal factorial
    nop

    j end


# $5 is output, $6 is input
factorial:
    fac_init:
        addiu $sp, $sp, -4
        sw $ra, 0($sp)

        bgtz $6, fac_mul
        nop

    fac_base:
        addiu $5, $0, 1
        j fac_return
        nop

    fac_mul:
        addiu $sp, $sp, -4
        sw $6, 0($sp)

        addiu $6, $6, -1
        jal factorial
        nop

        lw $6, 0($sp)
        addiu $sp, $sp, 4

        multu $5, $6
        mflo $5

    fac_return:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4

        jr $ra
        nop

end: