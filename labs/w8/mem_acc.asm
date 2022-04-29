# mem_acc.asm
# $Id: mem_acc.asm,v 1.1 2022/04/06 10:15:39 ahmet Exp ahmet $	

.set noreorder 					# Avoid reordering instructions 
.text 							# Start generating instructions 
.globl start 					# This label should be globally known
.ent start 						# This label marks an entry point  
	
start: 		
	lui 	$t1,	0x1001 		# Load upper half of memory address,
								# lower half is filled with zeros
	li		$s0,	0x10010004	# Load second memory address as 32-bit value
								# into $s0 using li pseudo-instruction
	li	 	$s1,	0x10010008	# Load third memory address into $s1
					
repeat:
	lw 		$t0, 	0x0($t1) 	# Load 32-bit word from the first memory address INTO register $8/$t0
	sw 		$t0, 	0x0($s0) 	# Write 32-bit word to the second memory address FROM register $8/$t0
	
	lhu 	$t2, 	0x0($t1) 	# Load 16-bit halfword (interpreted as unsigned) INTO register $t2
	sh 		$t2,	0x0($s1) 	# Write 16-bit halfword to third memory address FROM register $t2
	
	lbu 	$t3, 	0x3($t1)	# Load 8-bit byte (unsigned) from an offset of 3 from first memory address
	sb 		$t3, 	0x4($s1)	# Store 8-bit byte to the next word address after third memory address
	
	b repeat

.end start 						# Marks the end of the program

.data							# Items below this line will be stored in the
								# .data section of memory
					
.word		0x12345678			# Store the listed value(s) as 32 bit words
								# at the next available locations
