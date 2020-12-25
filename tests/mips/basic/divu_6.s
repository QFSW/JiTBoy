# desc: 30 / -7
#
# init: $1 = 30
# init: $2 = -7
# assert: $1 == 30
# assert: $2 == 0

divu $1, $2
mfhi $1
mflo $2