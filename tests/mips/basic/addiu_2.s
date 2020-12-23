# desc: adds over multiple registers
#
# init: $1 = 0
# assert: $1 == 10
# assert: $2 == 15
# assert: $3 == 16

addiu $1, $1, 10
addiu $2, $1, 5
addiu $3, $2, 1