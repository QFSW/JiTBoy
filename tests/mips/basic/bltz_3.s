; desc: bltz backwards taken
;
; init: $3 = -2
; init: $4 = 1
;
; assert: $3 == 4
; assert: $4 == 1

addi $3 $3 1
addi $3 $3 1
addi $3 $3 1
bltz $3 -12

addi $4 $0 1