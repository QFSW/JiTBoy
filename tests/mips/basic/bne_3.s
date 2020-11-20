; desc: bne backwards not taken
;
; init: $2 = 3
; init: $3 = 0
; init: $4 = 1
;
; assert: $3 == 3
; assert: $4 == 1

addi $3 $3 1
addi $3 $3 1
addi $3 $3 1
bne $3 $2 -12

addi $4 $0 1