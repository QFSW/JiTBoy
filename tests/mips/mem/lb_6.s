# desc: stores and loads in parts
#
# init: $sp = 0x20000000
# init: $1 = 0x12345678
# assert: $2 == 0x78
# assert: $3 == 0x56
# assert: $4 == 0x34
# assert: $5 == 0x12

sw $1, -4($sp)
lb $2, -1($sp)
lb $3, -2($sp)
lb $4, -3($sp)
lb $5, -4($sp)
nop