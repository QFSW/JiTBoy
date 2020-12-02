# desc: stores and loads max sized int32_t
#
# init: $sp = 0x20000000
# init: $1 = 0x7FFFFFFF
# assert: $2 == 0x7FFFFFFF

sw $1, -4($sp)
lw $2, -4($sp)
nop