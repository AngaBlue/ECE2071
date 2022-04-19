# Push stack frame
# Function will need 2 registers, therefore we should backup the registers to memory
subi $sp, $sp, 8
sw $t0, 4($sp)
sw $t1, 0($sp)

# g, h, i and j are stored in argument registers
add $t0, $a0, $a1
add $t1, $a2, $a3
sub $v0, $t0, $t1

# Pop stack frame
# Restore registers and rewind stack pointer
lw $t1, 0($sp)
lw $t0, 4($sp)
addi $sp, $sp, 8
jr $ra
