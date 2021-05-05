# desc: bgezal backwards taken with delay slot
#
# init: $3 = 3
# init: $4 = 1
# init: $5 = 0
#
# assert: $3 == -3
# assert: $4 == 1
# assert: $5 == 2

reset:
addi $3, $3, -1
addi $3, $3, -1
addi $3, $3, -1
bgezal $3, reset
addi $5, $5, 1

addi $4, $0, 1