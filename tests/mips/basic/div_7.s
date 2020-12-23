# desc: 0 / -7
#
# init: $1 = 0
# init: $2 = -7
# assert: $1 == 0
# assert: $2 == 0

div $1, $2
mfhi $1
mflo $2