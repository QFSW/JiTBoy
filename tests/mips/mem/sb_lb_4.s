# desc: loops over a 1 KiB of memory
#
# init: $sp = 0x20000000
# init: $1 = 1024
# init: $2 = 0
# init: $3 = 1
# assert: $6 == 0

write_setup:
    addi $6, $1, 0

write_loop:
    beq $2, $1, read_setup
    nop

    sub $4, $sp, $2
    addi $2, $2, 1
    sb $3, 0($4)

    j write_loop
    nop


read_setup:
    add $2, $0, $0

read_loop:
    beq $2, $1, end
    nop

    sub $4, $sp, $2
    addi $2, $2, 1
    lb $3, 0($4)
    nop

    sub $6, $6, $3

    j read_loop
    nop

end: