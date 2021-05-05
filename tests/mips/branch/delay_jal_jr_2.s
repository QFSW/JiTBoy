# desc: tests edge cases of delay slot with JAL
#
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
# init: $6 = 0
# init: $7 = 0
#
# assert: $3 == 0
# assert: $4 == 11
# assert: $5 == 0
# assert: $6 == 1
# assert: $7 == 1

start:
    jal func
    addi $5, $0, $ra
    j end
    addi $7, $7, 1

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    addi $6, $6, 1

end:
    addi $4, $4, 1
    sub $5, $5, $ra