; desc: bltz backwards taken
;
; init: $3 = 3
; init: $4 = 1
;
; assert: $3 == -3
; assert: $4 == 1

addi $3 $3 -1
addi $3 $3 -1
addi $3 $3 -1
bltz $3 -12

addi $4 $0 1