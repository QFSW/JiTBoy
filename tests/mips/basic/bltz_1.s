# desc: bltz not taken
#
# init: $1 = 0
# init: $3 = 0
# init: $4 = 1
# assert: $3 == 1
# assert: $4 == 1

bltz $1 8
addi $3 $0 1

addi $4 $0 1