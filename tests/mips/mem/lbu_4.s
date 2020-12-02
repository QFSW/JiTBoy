# desc: stores and loads max sized uint32_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFFFFFFFF
# assert: $2 == 0x000000FF

sw $1, -4($sp)
lbu $2, -1($sp)
nop