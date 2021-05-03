# desc: bne taken
#
# init: $1 = 0
# init: $2 = 1
# init: $3 = 0
# init: $4 = 0
#
# assert: $3 == 0
# assert: $4 == 1

bne $1, $2, skip
nop
addi $3, $0, 1

skip:
addi $4, $0, 1