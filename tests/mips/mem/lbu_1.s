# desc: stores and loads max sized int8_t
#
# init: $sp = 0x20000000
# init: $1 = 0x7F
# assert: $2 == 0x7F

sw $1, 0($sp)
lbu $2, 0($sp)
nop