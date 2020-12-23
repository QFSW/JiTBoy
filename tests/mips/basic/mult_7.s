# desc: 0x7FFFFFFF * 0x7FFFFFFF
#
# init: $1 = 0x7FFFFFFF
# init: $2 = 0x7FFFFFFF
# assert: $1 == 0x3FFFFFFF
# assert: $2 == 0x00000001

mult $1, $2
mfhi $1
mflo $2