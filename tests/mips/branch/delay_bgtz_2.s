# desc: bgtz not taken with delay slot
#
# init: $1 = -1
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
#
# assert: $3 == 1
# assert: $4 == 1
# assert: $5 == 1

bgtz $1, skip
addi $5, $5, 1
addi $3, $0, 1

skip:
addi $4, $0, 1