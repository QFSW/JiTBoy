# desc: 0xFFFFFFFF >> 31
#
# init: $1 = 0xFFFFFFFF
# init: $3 = 31
# assert: $2 == 0x00000001

srlv $2, $1, $3