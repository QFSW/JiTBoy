# desc: 0xFFFFFFFF * 0xFFFFFFFF
#
# init: $1 = 0xFFFFFFFF
# init: $2 = 0xFFFFFFFF
# assert: $1 == 0
# assert: $2 == 1

mult $1, $2
mfhi $1
mflo $2