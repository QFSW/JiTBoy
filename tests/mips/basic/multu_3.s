# desc: -5 * 1
#
# init: $1 = -5
# init: $2 = 1
# assert: $1 == 0
# assert: $2 == -5

multu $1, $2
mfhi $1
mflo $2