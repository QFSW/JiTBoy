# desc: 0x0000000F & 0xFFFFFFFF
#
# init: $1 = 0x0000000F
# assert: $3 == 0xFFFFFFF0

xori $3, $1, -1