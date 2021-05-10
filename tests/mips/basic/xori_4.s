# desc: 0x0000000F & 0xFFFF
#
# init: $1 = 0x0000000F
# assert: $3 == 0x0000FFF0

xori $3, $1, 0xFFFF