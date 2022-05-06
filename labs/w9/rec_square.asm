# $Id: rec_square.asm,v 1.1 2022/04/22 04:08:20 ahmet Exp ahmet $
# Written by Andy russell 20/03/2009

.set noreorder                              # Avoid reordering instructions
.text                                       # Start generating instructions
.globl start                                # This label should be globally known
.ent start                                  # This label marks an entry point

start:
    li      $a0,        3                   # load argument register $a0 with number to be squared
    jal     square                          # call the recursive function to calculate the square

infinite:
    b       infinite                        # wait here when the calculation has finished
                                            # $v0 contains the result

# ---------------------------------------------------------------
# square - input parameter $a0 contains number to be squared
# result returned in register $v0
# ---------------------------------------------------------------
square:
    subi    $sp,        $sp,    8           # decrement the stack pointer $sp
    sw      $ra,        4($sp)              # push the return address register $ra
    sw      $a0,        0($sp)              # push argument register $a0
    li      $t0,        1                   # load $t0 with 1 as part of test for base case
    bne     $a0,        $t0,    notbase     # branch if not the base case
    li      $v0,        1                   # return base result in $v0
    addi    $sp,        $sp,    8           # recover stack space (Note: did not corrupt registers)
    jr      $ra                             # jump to return address in $ra

notbase:
    subi    $a0,        $a0,    1           # load $a0 with n-1
    jal     square
    lw      $ra,        4($sp)              # pop stack
    lw      $a0,        0($sp)
    addi    $sp,        $sp,    8           # recover stack space
    add     $t0,        $a0,    $a0         # $t0 = 2n
    add     $v0,        $v0,    $t0         # $v0 = square(n-1) + 2n
    subi    $v0,        $v0,    1           # $v0 = square(n-1) + 2n - 1
    jr      $ra                             # jump to contents of return address register $ra

.end start                                  # Marks the end of the program
