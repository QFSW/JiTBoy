# desc: tests edge cases of delay slot (load in slot)
#
# init: $3 = 0
# init: $4 = 0
# init: $5 = 0
# init: $6 = 0
# init: $15 = 11
# init: $sp = 0x2000
#
# assert: $3 == 0
# assert: $4 == 11
# assert: $5 == 11
# assert: $6 == 1

start:
    sw $15, -4($sp)

    bgezal $0, func
    lw $5, -4($sp)

    j end
    nop

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    addi $6, $6, 1

end:
    addi $4, $4, 1