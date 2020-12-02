# desc: stores then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20
# assert: $3 == 21
# assert: $4 == 22

sb $1, -1($sp)
lb $2, -1($sp)
nop

addi $1, $2, 1
sb $1, -1($sp)
lb $3, -1($sp)
nop

addi $1, $3, 1
sb $1, -1($sp)
lb $4, -1($sp)
nop