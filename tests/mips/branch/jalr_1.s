# desc: uses jalr with custom link register
#
# init: $3 = 0
# init: $4 = 0
#
# assert: $3 == 0
# assert: $4 == 19

start:
    jal func
    nop

    add $4, $4, $1
    jr $1
    nop

junk:
    addi $3, $0, 1
    j end
    nop

func:
    addi $4, $4, 10
    jalr $1, $ra
    nop

    addi $4, $4, 7
    j end
    nop

end:
    addi $4, $4, 1