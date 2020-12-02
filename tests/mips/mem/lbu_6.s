# desc: stores and loads in parts
#
# init: $sp = 0x20000000
# init: $1 = 0x12345678
# assert: $2 == 0x78
# assert: $3 == 0x56
# assert: $4 == 0x34
# assert: $5 == 0x12

sw $1, -4($sp)
lbu $2, -1($sp)
lbu $3, -2($sp)
lbu $4, -3($sp)
lbu $5, -4($sp)
nop