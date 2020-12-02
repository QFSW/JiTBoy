# desc: stores and loads max sized int16_t
#
# init: $sp = 0x20000000
# init: $1 = 0x7FFF
# assert: $2 == 0x7FFF

sw $1, -4($sp)
lw $2, -4($sp)
nop