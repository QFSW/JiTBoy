# desc: moves to and from the hi register
#
# init: $1 = 1
# init: $2 = 0
# assert: $1 == 1
# assert: $2 == 1

mthi $1
mfhi $2