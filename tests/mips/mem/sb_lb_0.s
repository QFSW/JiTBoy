# desc: stores then loads a memory value
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20

sb $1, 0($sp)
lb $2, 0($sp)
nop