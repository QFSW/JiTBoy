# desc: stores then loads a memory value multiple times
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 22

sw $1, 0($sp)
lw $2, 0($sp)
nop

addi $1, $2, 1
sw $1, 0($sp)
lw $2, 0($sp)
nop

addi $1, $2, 1
sw $1, 0($sp)
lw $2, 0($sp)
nop