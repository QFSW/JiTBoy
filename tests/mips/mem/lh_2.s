# desc: stores and loads max sized uint16_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFFFF
# assert: $2 == -1

sw $1, -4($sp)
lh $2, -2($sp)
nop