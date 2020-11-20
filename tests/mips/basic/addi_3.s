; desc: adds a negative value into a zero initialized register
;
; init: $1 = 0
; assert: $1 == -10

addi $1 $1 -10