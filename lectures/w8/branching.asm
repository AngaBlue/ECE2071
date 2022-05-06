# Simple if statement, if s0 == s1, then s3 = s1 + s0
    bne     $s0,    $s1,    EndIf
    add     $s3,    $s1,    $s0
EndIf:

# Simple if/else statement, if s0 == s1, then s3 = s1 + s0 else s3 = s1 - s0
    bne     $s0,    $s1,    Else
    add     $s3,    $s1,    $s0
    j       EndIf
Else:
    sub     $s3,    $s1,    $s0
EndIf:

# if/else statement if s0 < s1, then s3 = s1 + s0 else s3 = s1 - s0
    slt     $t0,    $s0,    $s1
    beq     $t0,    $zero,  Else
    add     $s3,    $s1,    $s0
    j       EndIf
Else:
    sub     $s3,    $s1,    $s0
EndIf:

# equivalent statements
    move    $s1,    $s0
    add     $s1,    $zero,  $s0

    li      $s0,    255
    addi    $s0,    $zero,  255
                                    # or, for values using more than 16 bits
    lui     $s0,    0x00FF          # upper 16 bits
    ori     $s0,    $s0,    0xFFFF  # lower 16 bits
