# block 1
addi $4, $4, 55
addi $5, $5, 5
add $3, $4, $5
add $0, $4, $5
jal 284

# block 2
jr $0

# block 3
addi $3, $0, 0
addi $1, $0, 1
addi $6, $0, 6
jal 0x12c
jr $0


# func
addi $1, $1, 100
jr $ra