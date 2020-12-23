# desc: 5 * -1
#
# init: $1 = 5
# init: $2 = -1
# assert: $1 == 4
# assert: $2 == -1

multu $1, $2
mfhi $1
mflo $2