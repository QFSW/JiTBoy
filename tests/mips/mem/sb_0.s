# desc: stores in parts and loads
#
# init: $sp = 0x20000000
# init: $2 = 0x78
# init: $3 = 0x56
# init: $4 = 0x34
# init: $5 = 0x12
# assert: $1 == 0x12345678

sb $2, -1($sp)
sb $3, -2($sp)
sb $4, -3($sp)
sb $5, -4($sp)
lw $1, -4($sp)
nop