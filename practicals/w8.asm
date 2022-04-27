# Q1, a = $s0, b = $s1
# a = b + 100
addi    $s0,    $s1,    100
# a = b | 100
ori     $s0,    $s1,    100
# b = a ^ 100
xori    $s1,    $s0,    100
# b = b << 5
sll     $s1,    $s1,    5

# Q2
# The code will copy the null terminator at the end of the string because the condition to check if the
# character is a null terminator comes after it has been copied.

# Q3
