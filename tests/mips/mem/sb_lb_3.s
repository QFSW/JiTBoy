# desc: stores multiple memory values then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 21
# assert: $3 == 22
# assert: $4 == 23

addi $1, $1, 1
sb $1, 0($sp)

addi $1, $1, 1
sb $1, 1($sp)

addi $1, $1, 1
sb $1, 2($sp)

lb $2, 0($sp)
lb $3, 1($sp)
lb $4, 2($sp)
nop