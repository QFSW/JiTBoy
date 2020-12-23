# desc: adds a negative value into a zero initialized register
#
# init: $1 = 0
# assert: $1 == -10

addiu $1, $1, -10