# desc: 0x0000000F & 0xFFFFFFFF
#
# init: $1 = 0x0000000F
# init: $2 = 0xFFFFFFFF
# assert: $3 == 0xFFFFFFF0

xor $3, $1, $2