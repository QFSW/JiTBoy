# desc: loads into $zero
#
# init: $sp = 0x20000000
# init: $1 = 0xFF
# assert: $0 == 0

sw $1, -4($sp)
lw $0, -4($sp)
nop