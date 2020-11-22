# desc: loads an upper immediate into an initialized register
#
# init: $1 = 0x12345678
# assert: $1 == 0xFFFF0000

lui $1 0xFFFF