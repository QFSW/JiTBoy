# desc: -30 / 6
#
# init: $1 = -30
# init: $2 = 6
# assert: $1 == 0x00000004
# assert: $2 == 0x2AAAAAA5

divu $1, $2
mfhi $1
mflo $2