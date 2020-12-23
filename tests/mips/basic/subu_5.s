# desc: subtracts values into a register, checks optimisation when rs == $zero
#
# init: $1 = 1
# assert: $1 == -1

subu $1, $0, $1