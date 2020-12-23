# desc: 0xFF000000 << 4
#
# init: $1 = 0xFF000000
# init: $3 = 4
# assert: $2 == 0xF0000000

sllv $2, $1, $3