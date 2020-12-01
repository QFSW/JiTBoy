# desc: moves to and from the lo register
#
# init: $1 = 1
# init: $2 = 0
# assert: $1 == 1
# assert: $2 == 1

mtlo $1
mflo $2