# desc: 0xFFFFFFFF * 2
#
# init: $1 = 0xFFFFFFFF
# init: $2 = 2
# assert: $1 == -1
# assert: $2 == -2

mult $1, $2
mfhi $1
mflo $2