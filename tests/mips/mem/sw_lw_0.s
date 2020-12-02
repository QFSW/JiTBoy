# desc: stores then loads a memory value
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20

sw $1, -4($sp)
lw $2, -4($sp)
nop