# desc: 0xFFFFFFFF >> 31
#
# init: $1 = 0xFFFFFFFF
# assert: $2 == 0x00000001

srl $2, $1, 31