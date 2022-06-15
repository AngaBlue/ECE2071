.set noreorder                      # Avoid reordering instructions
.text                               # Start generating instructions
.globl start                        # The label should be globally known
.ent start                          # The label marks an entry point
start:
    li      $t0,    5               # Placeholder values
    li      $t1,    10              # Placeholder values
    li      $t2,    15              # Placeholder values
    bne     $t0,    $t1,    else    # i == j
    bne     $t0,    $t2,    else    # i == k
    addi    $t0,    $t0,    1       # i++
    j       endif
else:
    addi    $t2,    $t2,    1       # k++
endif:
    li      $t3,    16              # $t3 = 16
    sll     $t1,    $t0,    4       # 16 * i, as 2^4 = 16
    add     $t1,    $t1,    $t2     # j = j + k
.end start                          # Marks the end of the program
