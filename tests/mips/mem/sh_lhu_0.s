# desc: stores then loads a memory value
#
# init: $sp = 0x20000000
# init: $1 = 20
# assert: $2 == 20

sh $1, -2($sp)
lhu $2, -2($sp)
nop