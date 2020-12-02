# desc: stores and loads max sized int32_t
#
# init: $sp = 0x20000000
# init: $1 = 0x7FFFFFFF
# assert: $2 == 0x0000FFFF

sw $1, -4($sp)
lhu $2, -2($sp)
nop