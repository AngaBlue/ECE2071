.set noreorder                      # Avoid reordering instructions
.text                               # Start generating instructions
.globl start                        # This label should be globally known
.ent start                          # This label marks an entry point

start:
    li      $s0,    0               # a = 0
    li      $s1,    2               # b = 2
    li      $s2,    3               # c = 3

while:
    add     $s0,    $s0,    $s1     # a = a + b
    subi    $s2,    $s2,    1       # c = c - 1
    bgt     $s2,    $zero,  while   # if c > 0 goto while

.end start                          # Marks the end of the program
