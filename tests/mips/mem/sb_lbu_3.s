# desc: stores multiple memory values then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 21
# assert: $3 == 22
# assert: $4 == 23

addi $1, $1, 1
sb $1, -1($sp)

addi $1, $1, 1
sb $1, -2($sp)

addi $1, $1, 1
sb $1, -3($sp)

lbu $2, -1($sp)
lbu $3, -2($sp)
lbu $4, -3($sp)
nop