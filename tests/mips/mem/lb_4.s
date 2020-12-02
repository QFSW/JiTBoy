# desc: stores and loads max sized uint32_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFFFFFFFF
# assert: $2 == -1

sw $1, 0($sp)
lb $2, 0($sp)
nop