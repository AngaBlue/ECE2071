# add_sub.asm
# $Id: add_sub.asm,v 1.2 2022/04/06 10:11:34 ahmet Exp ahmet $

.set noreorder                          # Avoid reordering instructions
.text                                   # Start generating instructions
.globl start                            # This label should be globally known
.ent start                              # This label marks an entry point

start:
    li      $t0,        1               # Load the value 1
    li      $t1,        2               # Load the value 2
    add     $t2,        $t0,    $t1     # Add the values, store in $t2
    sub     $t3,        $t1,    $t0     # Subtract value 1 from value 2, store in $t3
infinite:
    j       infinite                    # an infinite loop so that the
                                        # computer will not execute code
                                        # past the end of the program
.end start                              # Marks the end of the program
