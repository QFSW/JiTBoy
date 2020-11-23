# desc: adds values into a register, checks optimisation when rt == $zero
#
# init: $1 = 1
# assert: $1 == 1

add $1, $1, $0