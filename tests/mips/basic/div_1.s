# desc: 30 / 50
#
# init: $1 = 30
# init: $2 = 50
# assert: $1 == 30
# assert: $2 == 0

div $1, $2
mfhi $1
mflo $2