# desc: edge cases of jalr (jumping) with delay slot
#
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
# init: $6 = 0
# init: $7 = 0
# init: $8 = 0
#
# assert: $3 == 0
# assert: $4 == 19
# assert: $5 == 1
# assert: $6 == 1
# assert: $7 == 0
# assert: $8 == 1
# assert: $ra == 12

start:
    jal func
    addi $5, $5, 1

    addi $4, $4, 1
    jr $1
    addi $6, $6, 1

junk:
    addi $3, $0, 1
    j end
    nop

func:
    addi $4, $4, 10
    jalr $1, $ra
    addi $ra, $0, 12

    addi $4, $4, 7
    j end
    addi $8, $8, 1

end:
    addi $4, $4, 1