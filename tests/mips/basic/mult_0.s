# desc: 5 * 6
#
# init: $1 = 5
# init: $2 = 6
# assert: $1 == 0
# assert: $2 == 30

mult $1, $2
mfhi $1
mflo $2