# desc: stores and loads in parts
#
# init: $sp = 0x20000000
# init: $1 = 0x12345678
# assert: $2 == 0x78
# assert: $2 == 0x56
# assert: $2 == 0x34
# assert: $2 == 0x12

sw $1, 0($sp)
lb $2, 0($sp)
lb $3, 1($sp)
lb $4, 2($sp)
lb $5, 3($sp)
nop