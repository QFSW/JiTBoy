# desc: 0x000000FF >> 4
#
# init: $1 = 0x000000FF
# init: $3 = 4
# assert: $2 == 0x0000000F

srlv $2, $1, $3