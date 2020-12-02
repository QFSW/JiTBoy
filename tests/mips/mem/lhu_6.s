# desc: stores and loads in parts
#
# init: $sp = 0x20000000
# init: $1 = 0x12345678
# assert: $2 == 0x5678
# assert: $3 == 0x1234

sw $1, -4($sp)
lhu $2, -2($sp)
lhu $3, -4($sp)
nop