# desc: stores and loads max sized uint8_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFF
# assert: $2 == -1

sw $1, 0($sp)
lb $2, 0($sp)
nop