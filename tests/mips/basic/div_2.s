# desc: 30 / 7
#
# init: $1 = 30
# init: $2 = 7
# assert: $1 == 2
# assert: $2 == 4

div $1, $2
mfhi $1
mflo $2