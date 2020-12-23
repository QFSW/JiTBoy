# desc: tests that $ra is set when the branch is not taken
#
# init: $2 = -1
# init: $3 = 0
# init: $4 = 0
# assert: $3 == 0
# assert: $4 == 1
# assert: $5 == 12

start:
    bgezal $2, func
    nop
    addi $5, $ra, 0
    bgezal $2, func
    nop
    sub $5, $ra, $5
    j end
    nop

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    nop

end:
    addi $4, $4, 1