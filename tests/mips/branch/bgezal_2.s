# desc: bgezal not taken
#
# init: $1 = -1
# init: $3 = 0
# init: $4 = 1
# assert: $3 == 1
# assert: $4 == 1

bgezal $1, skip
nop
addi $3, $0, 1

skip:
addi $4, $0, 1