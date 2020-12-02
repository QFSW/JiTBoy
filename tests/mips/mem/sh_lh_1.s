# desc: stores then loads a memory value multiple times
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 22

sh $1, -2($sp)
lh $2, -2($sp)
nop

addi $1, $2, 1
sh $1, -2($sp)
lh $2, -2($sp)
nop

addi $1, $2, 1
sh $1, -2($sp)
lh $2, -2($sp)
nop