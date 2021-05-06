# desc: lwl to zero initialized register
#
# init: $sp = 0x20000000
# init: $1 = 0x00000000
# init: $2 = 0x01234567
# init: $3 = 0x89ABCDEF
#
# assert: $1 == 0x23456700

setup:
    sw $2, 0($sp)
    sw $3, 4($sp)    

test:
    lwl $1, 1($sp)
    nop