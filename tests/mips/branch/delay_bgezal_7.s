# desc: tests that $ra is set when the branch is not taken
#
# init: $2 = -1
# init: $3 = 0
# init: $4 = 0
# init: $15 = 0
#
# assert: $3 == 0
# assert: $4 == 1
# assert: $5 == 12
# assert: $15 == 3

start:
    bgezal $2, func
    addi $15, $15, 1

    addi $5, $ra, 0
    bgezal $2, func
    addi $15, $15, 1

    sub $5, $ra, $5
    j end
    addi $15, $15, 1

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    addi $15, $15, 1

end:
    addi $4, $4, 1