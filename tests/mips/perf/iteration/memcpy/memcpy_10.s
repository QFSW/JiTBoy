# name: memcpy(1024)
# desc: copies 1024 bytes of memory to the destination
#
# init: $1 = 0x1000000
# init: $2 = 0xF000000
# init: $3 = 1024

setup:
    jal memcpy
    nop

    j end


# $1 = src
# $2 = dst
# $3 = words
memcpy:
    lbu $10, 0($1)
    nop

    sb $10, 0($2)
    addiu $1, $1, 1
    addiu $2, $2, 1
    addiu $3, $3, -1
    bgtz $3, memcpy
    nop

    jr $ra
    nop

end: