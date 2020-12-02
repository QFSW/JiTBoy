# desc: stores multiple memory values then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 21
# assert: $3 == 22
# assert: $4 == 23

addi $1, $1, 1
sh $1, -2($sp)

addi $1, $1, 1
sb $1, -4($sp)

addi $1, $1, 1
sb $1, -6($sp)

lh $2, -2($sp)
lh $3, -4($sp)
lh $4, -6($sp)
nop