    strcpy:                             # a0 = dest, a1 = src
# Create stack frame
    subi    $sp,    $sp,    4
    sw      $s0,    0($sp)
    addi    $s0,    $zero,  0           # i = 0
While:
    add     $t0,    $a1,    $s0         # address = src + i
    lb      $t1,    0($t0)              # load char from src into t1
    add     $t0,    $a0,    $s0         # address = dest + i
    sb      $t1,    0($t0)              # store char to dest from t1
    beq     $t1,    $zero,  EndWhile    # if char == NULL, break
    addi    $s0,    $s0,    1           # ++i
    j       While
EndWhile:
                                        # Pop stack frame
    lw      $s0,    0($sp)
    addi    $sp,    $sp,    4
    jr      $ra
