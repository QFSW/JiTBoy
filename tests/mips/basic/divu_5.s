# desc: -30 / -6
#
# init: $1 = -30
# init: $2 = -6
# assert: $1 == 0x00000000
# assert: $2 == 0xFFFFFFE2

divu $1, $2
mfhi $1
mflo $2