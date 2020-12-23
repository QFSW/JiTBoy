# desc: 0x000000FF >> 4
#
# init: $1 = 0x000000FF
# assert: $2 == 0x0000000F

srl $2, $1, 4