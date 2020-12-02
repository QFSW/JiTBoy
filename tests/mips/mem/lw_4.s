# desc: stores and loads max sized uint32_t
#
# init: $sp = 0x20000000
# init: $1 = 0xFFFFFFFF
# assert: $2 == 0xFFFFFFFF

sw $1, -4($sp)
lw $2, -4($sp)
nop