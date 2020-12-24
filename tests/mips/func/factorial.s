# desc: recursive factorial using stack, computes 10!
#
# init: $6 = 10
# assert: $5 == 3628800

setup:
    lui $sp, 0x2000

    jal factorial
    nop

    j end


# $5 is output, $6 is input
factorial:
    fac_init:
        sw $ra, 0($sp)
        addiu $sp, $sp, 4

        bgtz $6, fac_mul

    fac_base:
        addiu $5, $0, 1
        j fac_return
        nop

    fac_mul:
        sw $6, 0($sp)
        addiu $sp, $sp, 4

        addiu $6, $6, -1
        jal factorial
        nop

        addiu $sp, $sp, -4
        lw $6, 0($sp)
        nop

        multu $5, $6
        mflo $5

    fac_return:
        addiu $sp, $sp, -4
        lw $ra, 0($sp)
        nop

        jr $ra
        nop

end: