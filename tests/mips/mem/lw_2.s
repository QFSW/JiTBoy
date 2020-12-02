# desc: stores and loads max sized uint16_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFFFF
# assert: $2 == 0xFFFF

sw $1, 0($sp)
lw $2, 0($sp)
nop