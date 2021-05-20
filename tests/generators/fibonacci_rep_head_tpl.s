# name: fibonacci_rep[$NUM]($INPUT)
# desc: recursive fibonacci using stack, computes fib($INPUT). fibonacci code repeated $NUM times
#
# init: $sp = 0x20000000
# init: $10 = $INPUT
# assert: $11 == $OUTPUT