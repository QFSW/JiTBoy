# desc: 5 * 0
#
# init: $1 = 5
# init: $2 = 0
# assert: $1 == 0
# assert: $2 == 5

mult $1, $2
mfhi $1
mflo $2