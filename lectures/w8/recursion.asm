main:
    addi    $a0,    $zero,  10  # Calculate 5!
    jal     fact
                                # Print
    move    $a0,    $v0
    li      $v0,    1
    syscall 
                                # Exit
    li      $v0,    10          # Terminate program run and exit
    syscall 

fact:
    subi    $sp,    $sp,    8   # Store 2 registers on the stack
    sw      $ra,    4($sp)
    sw      $a0,    0($sp)
    blt     $a0,    1,      L1  # Branch if n <= 1
    subi    $a0,    $a0,    1   # n = n - 1
    jal     fact
    lw      $a0,    0($sp)      # Restore registers from stack
    lw      $ra,    4($sp)
    addi    $sp,    $sp,    8   # Pop stack
    mult    $v0,    $a0         # Multiply n by result
    mflo    $v0
    jr      $ra
    L1:                         # n <= 1
    addi    $v0,    $zero,  1   # Return 1
    addi    $sp,    $sp,    8   # Pop stack
    jr      $ra
