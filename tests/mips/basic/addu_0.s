# desc: adds values into a zero initialized register
#
# init: $1 = 0
# init: $2 = 10
# init: $3 = 1
# assert: $1 == 11

addu $1, $2, $3