# desc: bgtz not taken
#
# init: $1 = -1
# init: $3 = 0
# init: $4 = 1
# assert: $3 == 1
# assert: $4 == 1

bgtz $1 12
nop
addi $3 $0 1

addi $4 $0 1