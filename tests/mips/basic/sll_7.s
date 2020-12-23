# desc: 0xFFFFFFFF << 31
#
# init: $1 = 0xFFFFFFFF
# assert: $2 == 0x80000000

sll $2, $1, 31