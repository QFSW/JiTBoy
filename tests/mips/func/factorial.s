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
    fac-init:
        sw $ra, 0($sp)
        addiu $sp, $sp, 4

        bgtz $6, fac-mul

    fac-base:
        addiu $5, $0, 1
        j fac-return
        nop

    fac-mul:
        sw $6, 0($sp)
        addiu $sp, $sp, 4

        addiu $6, $6, -1
        jal factorial
        nop

        addiu $sp, $sp, -4
        lw $6, 0($sp)

        multu $5, $6
        mflo $5

    fac-return:
        addiu $sp, $sp, -4
        lw $ra, 0($sp)

        jr $ra
        nop

end: