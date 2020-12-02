# desc: stores then loads multiple memory values
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20
# assert: $3 == 21
# assert: $4 == 22

sh $1, -2($sp)
lh $2, -2($sp)
nop

addi $1, $2, 1
sh $1, -4($sp)
lh $3, -4($sp)
nop

addi $1, $3, 1
sh $1, -6($sp)
lh $4, -6($sp)
nop