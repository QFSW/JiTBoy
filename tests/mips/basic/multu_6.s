# desc: 0xFFFFFFFF * 0xFFFFFFFF
#
# init: $1 = 0xFFFFFFFF
# init: $2 = 0xFFFFFFFF
# assert: $1 == 0xFFFFFFFE
# assert: $2 == 0x00000000

multu $1, $2
mfhi $1
mflo $2