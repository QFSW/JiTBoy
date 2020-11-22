# desc: adds over multiple registers
#
# init: $1 = 0
# assert: $1 == 10
# assert: $2 == 15
# assert: $3 == 16

addi $1 $1 10
addi $2 $1 5
addi $3 $2 1