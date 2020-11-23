# desc: subtracts values into a register, checks optimisation when rs == $zero
#
# init: $1 = 1
# assert: $1 == -1

sub $1, $0, $1