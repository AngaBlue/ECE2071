strcpy:
    # Create stack frame
    subi    $sp,    $sp,    4
    sw		$s0,    0($sp)
    addi	$s0,    $zero,  1
While:
    add		$t0,    $a1,    $s1     # t0 = dest + i
        
    
