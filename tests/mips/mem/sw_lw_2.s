# desc: stores then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20
# assert: $3 == 21
# assert: $4 == 22

sw $1, -4($sp)
lw $2, -4($sp)
nop

addi $1, $2, 1
sw $1, -8($sp)
lw $3, -8($sp)
nop

addi $1, $3, 1
sw $1, -12($sp)
lw $4, -12($sp)
nop