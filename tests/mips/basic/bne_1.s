# desc: bne not taken
#
# init: $1 = 0
# init: $2 = 0
# init: $3 = 0
# init: $4 = 1
# assert: $3 == 1
# assert: $4 == 1

bne $1 $2 12
nop
addi $3 $0 1

addi $4 $0 1