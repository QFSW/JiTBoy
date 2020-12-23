# desc: 30 / -6
#
# init: $1 = 30
# init: $2 = -6
# assert: $1 == 30
# assert: $2 == 0

divu $1, $2
mfhi $1
mflo $2