.set noreorder 					        # Avoid reordering instructions 
.text 							        # Start generating instructions 
.globl start 					        # This label should be globally known
.ent start 						        # This label marks an entry point  
	
start:
    li      $t0,    0x10010000	        # Load the value 4 into $v0
    lb		$s0,    0($t0)		        # Load byte into $s0
    ori		$s0,    $s0,    0x00000003	# Set 2 least significant bits to 1 
    andi	$s0,    $s0,    0xFFFFFFF3	# Set next 2 least significant bits to 0 
    xori    $s0,    $s0,    0x000000F0	# Invert next 4 least significant bits
    li      $t0,    0x10010005	        # Load the value 4 into $v0
    sb		$s0,    0($t0)		        # Store byte into address in $t0
infinite:
	j		infinite				

.end start 						        # Marks the end of the program
