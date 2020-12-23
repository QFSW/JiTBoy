# desc: adds values into a register, checks optimisation when rd == rs
#
# init: $1 = 1
# init: $2 = 10
# assert: $1 == 11

addu $1, $1, $2