# desc: adds values into a register, checks optimisation when rs == $zero
#
# init: $1 = 1
# assert: $1 == 1

addu $1, $0, $1