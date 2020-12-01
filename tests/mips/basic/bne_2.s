# desc: bne backwards taken
#
# init: $2 = 6
# init: $3 = 0
# init: $4 = 1
#
# assert: $3 == 6
# assert: $4 == 1

reset:
addi $3, $3, 1
addi $3, $3, 1
addi $3, $3, 1
bne $3, $2, reset
nop

addi $4, $0, 1