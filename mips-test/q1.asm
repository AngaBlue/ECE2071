# Find the smallest hexadecimal digit in $a0 and write the output to $v0.
# $a0 = [in] hexadecimal digits
# $s0 = iteration counter
# $s1 = bit mask
# $s2 = loop limit
# $v0 = [out] lowest hex digit

.set noreorder                              # Avoid reordering instructions
.text                                       # Start generating instructions
.globl start                                # Makes start globally known
.ent start                                  # Marks the entry point

start:
    move    $s0,    $zero                   # Initialise counter at 0
    li      $s1,    0xF                     # Initialise bit mask to 1 byte
    li      $s2,    8                       # Initialise loop count to 8 for each digit
    li      $v0,    0xF                     # Initialise smallest hex digit to the largest possible value; F

loop:
    and     $t0,    $a0,    $s1             # Find lowest 4 bits using bit mask
    bge     $t0,    $v0,    not_smallest    # If lowest 4 bits are greater than or equal to the current smallest digit, skip to not_smallest
    move    $v0,    $t0                     # Set v0 to lowest 4 bits

not_smallest:
    addi    $s0,    $s0,    1               # Increment counter
    srl     $a0,    $a0,    4               # Shift right by 4 bits
    bne     $s0,    $s2,    loop            # Loop until counter is has run through all 8 bytes

.end start                                  # Marks the end of the program
