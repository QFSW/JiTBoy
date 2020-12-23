# desc: tests that $ra is set
#
# init: $3 = 0
# init: $4 = 0
# assert: $3 == 0
# assert: $4 == 11

start:
    bgezal $0, func
    nop
    j end
    nop

junk:
    addi $3, $0, 1

func:
    addi $4, $4, 10
    jr $ra
    nop

end:
    addi $4, $4, 1