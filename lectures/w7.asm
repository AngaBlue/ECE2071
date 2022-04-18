add     $t0, $t1, $t2   # $t0 = $t1 + $t2

lw		$t1, 0($s1)		# load from $s1 into $t1
sw		$t1, 4($s1)		# store $t1 into $s1+4

# f = (g + h) – (i + j);

add     $t0, $s1, $s2 # register $t0 contains g + h
add     $t1, $s3, $s4 # register $t1 contains i + j
sub     $s0, $t0, $t1 # f gets $t0 – $t1, which is (g + h)–(i + j)

# if/else control flow, i j k = $s0, $s1, $s2
blt		$s0, $s1, Then	# if i < j
addu    $s2, $s2, $s1	# k = k + j
j		EndIf           # jump to end of if/else
Then:
addu    $s2, $s2, $s0	# k = k + i
EndIf:

# while loop, run 10 times
li      $s0, 0      # i = 0
li		$t1, 9		# $t1 = 9
Loop:
beq     $s0, $t1, EndLoop	# if i == 9
addi	$s0, $s0, 1	# i++
j       Loop
EndLoop:
