# name: memcpyw(128)
# desc: copies 128 words of memory to the destination
#
# init: $1 = 0x1000000
# init: $2 = 0xF000000
# init: $3 = 128

setup:
    jal memcpyw
    nop

    j end


# $1 = src
# $2 = dst
# $3 = words
memcpyw:
    lw $10, 0($1)
    nop

    sw $10, 0($2)
    addiu $1, $1, 4
    addiu $2, $2, 4
    addiu $3, $3, -1
    bgtz $3, memcpyw
    nop

    jr $ra
    nop

end: