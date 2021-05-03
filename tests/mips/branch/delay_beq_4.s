# desc: beq backwards taken with delay slot
#
# init: $2 = 3
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
#
# assert: $3 == 6
# assert: $4 == 0
# assert: $5 == 2

reset:
addi $3, $3, 1
addi $3, $3, 1
addi $3, $3, 1
beq $3, $2, reset
addi $5, $5, 1