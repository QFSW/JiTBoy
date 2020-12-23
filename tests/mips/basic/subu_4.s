# desc: subtracts values into a register, checks optimisation when rt == $zero
#
# init: $1 = 1
# assert: $1 == 1

subu $1, $1, $0