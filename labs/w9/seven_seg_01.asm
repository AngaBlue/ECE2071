# $Id: seven_seg_01.asm,v 1.1 2022/04/22 04:04:48 ahmet Exp ahmet $

.set noreorder                              # Avoid reordering instructions
.text                                       # Start generating instructions
.globl start                                # This label should be globally known
.ent start                                  # This label marks an entry point

start:
    li      $t0,    0xFFFF0010              # Load memory address of seven segment display
    li      $t1,    0x1                     # Initialise value to write to display

repeat:
    sll     $t1,    $t1,        1           # $t0 = $t1 << 1
    li      $t2,    0x40                    # $t2 = 0100 0000
    bne     $t1,    $t2,        notlarge    # If $t1 != 0000 0000, go to end
    li      $t1,    0x1                     # $t1 = 1
notlarge:
    sb      $t1,    0($t0)                  # Write the value 0x1 to seven segment display
    b       repeat                          # Branch to repeat label

.end start                                  # Marks the end of the program
