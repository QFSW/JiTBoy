# desc: lwl to zero initialized register
#
# init: $sp = 0x20000000
# init: $1 = 0
# init: $2 = 0x00112233
# init: $3 = 0x44556677
#
# assert: $1 == 0x22330000

setup:
    sw $2, 0($sp)
    sw $3, 4($sp)    

test:
    lwl $1, 2($sp)
    nop