# desc: bgtz backwards taken
#
# init: $3 = 4
# init: $4 = 1
#
# assert: $3 == -2
# assert: $4 == 1

reset:
addi $3, $3, -1
addi $3, $3, -1
addi $3, $3, -1
bgtz $3, reset
nop

addi $4, $0, 1