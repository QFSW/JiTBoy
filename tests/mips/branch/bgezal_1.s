# desc: bgezal taken
#
# init: $1 = 0
# init: $3 = 0
# init: $4 = 1
#
# assert: $3 == 0
# assert: $4 == 1

bgezal $1, skip
nop
addi $3, $0, 1

skip:
addi $4, $0, 1