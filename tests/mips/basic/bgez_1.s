# desc: bgez taken
#
# init: $1 = 0
# init: $3 = 0
# init: $4 = 1
# assert: $3 == 0
# assert: $4 == 1

bgez $1 12
nop
addi $3 $0 1

addi $4 $0 1