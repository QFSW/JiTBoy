# desc: adds a value into a register multiple times
#
# init: $1 = 0
# assert: $1 == 50

addiu $1, $1, 10
addiu $1, $1, 10
addiu $1, $1, 10
addiu $1, $1, 10
addiu $1, $1, 10