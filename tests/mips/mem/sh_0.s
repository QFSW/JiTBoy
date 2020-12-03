# desc: stores in parts and loads
#
# init: $sp = 0x20000000
# init: $2 = 0x5678
# init: $3 = 0x1234
# assert: $1 == 0x12345678

sh $2, -2($sp)
sh $3, -4($sp)
lw $1, -4($sp)
nop