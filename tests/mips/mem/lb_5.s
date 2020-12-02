# desc: stores and loads max sized int32_t
#
# init: $sp = 0x20000000
# init: $1 = 0x7FFFFFFF
# assert: $2 == -1

sw $1, -4($sp)
lb $2, -1($sp)
nop