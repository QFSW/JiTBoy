# name: primal(67)
# desc: simple iterative primal test
#
# init: $6 = 67
# assert: $5 == 1

setup:
    jal primal
    nop

    j end


# $5 is output, $6 is input
primal:
    primal_init:
        addi $4, $6, -2
        blez $4, primal_true
        addiu $1, $0, 2

    primal_loop:
        divu $6, $1
        mfhi $2
        beq $2, $0, primal_false
        nop

        addiu $1, $1, 1
        bne $1, $6, primal_loop
        nop

    primal_true:
        addiu $5, $0, 1
        jr $ra
        nop

    primal_false:
        addiu $5, $0, 0
        jr $ra
        nop
end: