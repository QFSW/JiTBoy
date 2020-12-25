# name: unroll(512/16)
# desc: loop of 512 unrolled into chunks of 32
#
# init: $1 = 0
# init: $2 = 512
# assert: $1 == 512

start:
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
addi $1, $1, 1
bne $1, $2, start
nop
