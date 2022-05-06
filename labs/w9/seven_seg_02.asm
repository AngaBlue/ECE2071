# $Id: seven_seg_02.asm,v 1.1 2022/04/22 04:06:05 ahmet Exp ahmet $

.set noreorder                              # Avoid reordering instructions
.text                                       # Start generating instructions
.globl start                                # This label should be globally known
.ent start                                  # This label marks an entry point

start:
    li      $t0,        0xFFFF0010          # Load memory address of seven segment display
    li      $t1,        0x10010000          # Load base memory address of decoder values array
    li      $a0,        0x5                 # Initialise value to write to display
    li      $s0,        0                   # Initialise loop counter

repeat:
    addi    $s0,        $s0,        1       # Increment loop counter
    bne     $s0,        $a0,        skip    # If loop counter is not equal to value
    li      $s0,        0
skip:
    jal     draw_num                        # Call the draw_num function using 'jump and link'
    b       repeat

end:
    b       end                             # End at infinite loop

draw_num:
    add     $t2,        $t1,        $s0     # Calculate address of element in array to access
    lbu     $t3,        0($t2)              # Load decoder value byte from memory into $t3
    sb      $t3,        0($t0)              # Write decoder value to seven segment display
    jr      $ra                             # Return from function using 'jump register' and $ra

.end start                                  # Marks the end of the program

.data                                       # Items below this line will be stored in the
                                            # .data section of memory

# Store the listed value(s) as 8-bit values at the next available locations
.byte 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
