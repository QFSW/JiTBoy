# desc: subtracts values into a register, checks optimisation when rd == rt
#
# init: $1 = 1
# init: $2 = 10
# assert: $1 == 9

sub $1, $2, $1