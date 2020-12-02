# desc: stores then loads a memory value
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20

sb $1, -1($sp)
lbu $2, -1($sp)
nop