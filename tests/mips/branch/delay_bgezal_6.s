# desc: tests that $ra is set with delay slots
#
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
# init: $6 = 0
#
# assert: $3 == 0
# assert: $4 == 11
# assert: $5 == 1
# assert: $6 == 1

start:
    bgezal $0, func
    addi $5, $5, 1
    j end
    nop

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    addi $6, $6, 1

end:
    addi $4, $4, 1