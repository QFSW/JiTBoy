# desc: basic long multiply algorithm without carry using optimisations
#
# init: $1 = 20
# init: $2 = 30
# assert: $3 == 600

setup:
    beq $0, $1, skip
    addi $3, $0, 0

    beq $0, $2, skip
    nop

start:
    addi $1, $1, -1
    bne $0, $1, start
    add $3, $3, $2

skip: