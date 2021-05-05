# desc: tests edge cases of delay slot with JR
#
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
# init: $6 = 0
# init: $7 = 0
#
# assert: $3 == 0
# assert: $4 == 11
# assert: $5 == 1
# assert: $6 == 0
# assert: $7 == 1
# assert: $ra == 123

start:
    jal func
    addi $5, $5, 1
    j end
    addi $7, $7, 1

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    addi $ra, $0, 123

end:
    addi $4, $4, 1