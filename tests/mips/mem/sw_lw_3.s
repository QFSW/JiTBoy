# desc: stores multiple memory values then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 21
# assert: $3 == 22
# assert: $4 == 23

addi $1, $1, 1
sw $1, -4($sp)

addi $1, $1, 1
sw $1, -8($sp)

addi $1, $1, 1
sw $1, -12($sp)

lw $2, -4($sp)
lw $3, -8($sp)
lw $4, -12($sp)
nop