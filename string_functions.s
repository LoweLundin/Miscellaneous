##############################################################################
#
#  KURS: 1DT038 2018.  Computer Architecture I
#	
#  DATUM: 13/9 - 2018
#
#  NAMN: Lowe Lundin
#
#  NAMN:
#
##############################################################################

	.data
	
ARRAY_SIZE:
	.word	10	# Change here to try other values (less than 10)
FIBONACCI_ARRAY:
	.word	1, 1, 2, 3, 5, 8, 13, 21, 34, 55
STR_str:
	.asciiz "Hunden, Katten, Glassen"
REVERSE_STRING: 
	.asciiz "Malmo FF"

	.globl DBG
	.text

##############################################################################
#
# DESCRIPTION:  For an array of integers, returns the total sum of all
#		elements in the array.
#
# INPUT:        $a0 - address to first integer in array.
#		$a1 - size of array, i.e., numbers of integers in the array.
#
# OUTPUT:       $v0 - the total sum of all integers in the array.
#
##############################################################################
integer_array_sum:  

DBG:	##### DEBUGG BREAKPOINT ######

        addi    $v0, $zero, 0           # Initialize Sum to zero.
	add	$t0, $zero, $zero	# Initialize array index i to zero.
	
for_all_in_array:

	#### Append a MIPS-instruktion before each of these comments
	
	beq $t0 $a1 end_for_all # Done if i == N
	sll $t1 $t0 2 # 4*i
	add $t2 $a0 $t1 # address = ARRAY + 4*i
	lb $t2 0($t2) # n = A[i]
    add $v0 $v0 $t2 # Sum = Sum + n
    addi $t0 $t0 1 # i++ 
  	j for_all_in_array # next element
	
end_for_all:
	
	jr	$ra			# Return to caller.
	
##############################################################################
#
# DESCRIPTION: Gives the length of a string.
#
#       INPUT: $a0 - address to a NUL terminated string.
#
#      OUTPUT: $v0 - length of the string (NUL excluded).
#
#    EXAMPLE:  string_length("abcdef") == 6.
#
##############################################################################	
string_length:
	addi $v0 $zero 0 # Length of the string declared to zero
	addi $t0 $zero 0 # Array index i = 0

string_length_loop:

	lb $t0 0($a0) #Load the current byte into t0
	beq $t0 $zero string_length_end #Go to string_length_end if t0 is null
	addi $a0 $a0 1 #Increment a0 by one to go to next byte in string
	addi $v0 $v0 1 #Increment the string length count by 1
	j string_length_loop

string_length_end:

	jr	$ra
	
##############################################################################
#
#  DESCRIPTION: For each of the characters in a string (from left to right),
#		call a callback subroutine.
#
#		The callback suboutine will be called with the address of
#	        the character as the input parameter ($a0).
#	
#        INPUT: $a0 - address to a NUL terminated string.
#
#		$a1 - address to a callback subroutine.
#
##############################################################################	
string_for_each:

	addi	$sp, $sp, -4		# PUSH return address to caller
	sw	$ra, 0($sp)
	addi $t0 $zero 0 # Array index i = 0

string_for_each_loop:

	lb $t0 0($a0) #Load the current byte into t0
	beq $t0 $zero string_for_each_end #Go to string_length_end if t0 is null
	addi $sp $sp -4 # Push address of string
	sw $a0 0($sp)
	jal $a1 #Jump to the address of desired callback subroutine
	lw $a0 0($sp) #Pop address of string
	addi $sp $sp 4
	addi $a0 $a0 1 #Increment a0 by one to go to next byte in string
	j string_for_each_loop 	

string_for_each_end:

	lw	$ra, 0($sp)		# Pop return address to caller
	addi	$sp, $sp, 4		

	jr	$ra

##############################################################################
#
#  DESCRIPTION: Transforms a lower case character [a-z] to upper case [A-Z].
#	
#        INPUT: $a0 - address of a character 
#
##############################################################################		
to_upper:

	lb $v0 0($a0) #Load the character into v0
	addi $t1 $zero 97 #Sets t1 to 97(decimal system), which is where lower case a is located in the ascii table
	addi $t2 $zero 123 #Sets t2 to 123(decimal system), which is where lower case z is located in the ascii table
	slt $t3 $v0 $t1 #Checks if v0 has a higher ascii value than t1
	slt $t4 $t2 $v0 #Checks if v0 has a lower ascii value than t2
	add $t5 $t4 $t3 #Adds the two values
	bne $t5 $zero to_upper_end #Jumps to to_upper_end if the values added were not zero, like an OR-gate
	addi $v0 $v0 -32 #"Steps down" the ascii value of the character by 32, which is were the corresponding upper case letter is located
	sb $v0,0($a0) #Saves v0 into a0

to_upper_end:
	jr	$ra


##############################################################################
#
#  DESCRIPTION: Reverses a string 
#	
#       INPUT: $a0 - address to a NUL terminated string.
#
##############################################################################		
reverse_string:

	addi	$sp, $sp, -4		# PUSH return address to caller
	sw	$ra, 0($sp)
	addi	$sp, $sp, -4
	sw $a0 0($sp) #Making sure to save before jumping to string_length
	jal string_length
	lw $a0 0($sp) #Now loading a0
	addi $sp $sp 4
	addi $v0 $v0 -1 #Decreases v0 by 1 as the start index is 0
	add $t0 $a0 $v0 #Saves the adress of the string end in t0

reverse_string_loop:

	lb $t1 0($a0) #Loads first char
	lb $t2 0($t0) #Loads last char
	sb $t1 0($t0) #Saves first char to the adress of the last character
	sb $t2 0($a0) #Saves last char to the adress of the first character

	addi $a0 $a0 1 #Increments a0 by one to go on to the next character
	addi $t0 $t0 -1 #Decreases t0 by one to go on to the previous character
	slt $t3 $t0 $a0 #Checks if t0 is less than a0
	beq $t3 $zero reverse_string_loop 
	
	lw	$ra, 0($sp)		# Pop return address to caller
	addi	$sp, $sp, 4		

	jr $ra

##############################################################################
#
# Strings used by main:
#
##############################################################################

	.data

NLNL:	.asciiz "\n\n"
	
STR_sum_of_fibonacci_a:	
	.asciiz "The sum of the " 
STR_sum_of_fibonacci_b:
	.asciiz " first Fibonacci numbers is " 

STR_string_length:
	.asciiz	"\n\nstring_length(str) = "

STR_for_each_ascii:	
	.asciiz "\n\nstring_for_each(str, ascii)\n"

STR_for_each_to_upper:
	.asciiz "\n\nstring_for_each(str, to_upper)\n\n"	

STR_string_reverse:
	.asciiz "\n\nstring_reverse: Malmo FF\n\n"

	.text
	.globl main

##############################################################################
#
# MAIN: Main calls various subroutines and print out results.
#
##############################################################################	
main:
	addi	$sp, $sp, -4	# PUSH return address
	sw	$ra, 0($sp)

	##
	### integer_array_sum
	##
	
	li	$v0, 4
	la	$a0, STR_sum_of_fibonacci_a
	syscall

	lw 	$a0, ARRAY_SIZE
	li	$v0, 1
	syscall

	li	$v0, 4
	la	$a0, STR_sum_of_fibonacci_b
	syscall
	
	la	$a0, FIBONACCI_ARRAY
	lw	$a1, ARRAY_SIZE
	jal 	integer_array_sum

	# Print sum
	add	$a0, $v0, $zero
	li	$v0, 1
	syscall

	li	$v0, 4
	la	$a0, NLNL
	syscall
	
	la	$a0, STR_str
	jal	print_test_string

	##
	### string_length 
	##
	
	li	$v0, 4
	la	$a0, STR_string_length
	syscall

	la	$a0, STR_str
	jal 	string_length

	add	$a0, $v0, $zero
	li	$v0, 1
	syscall

	##
	### string_for_each(string, ascii)
	##
	
	li	$v0, 4
	la	$a0, STR_for_each_ascii
	syscall
	
	la	$a0, STR_str
	la	$a1, ascii
	jal	string_for_each

	##
	### string_for_each(string, to_upper)
	##
	
	li	$v0, 4
	la	$a0, STR_for_each_to_upper
	syscall

	la	$a0, STR_str
	la	$a1, to_upper
	jal	string_for_each
	
	la	$a0, STR_str
	jal	print_test_string

	##
	### reverse_string(string)
	##

	li $v0 4
	la $a0 STR_string_reverse
	syscall

	li $v0 4
	la $a0 REVERSE_STRING
	jal reverse_string

	la $a0 REVERSE_STRING
	jal print_test_string
	
	lw	$ra, 0($sp)	# POP return address
	addi	$sp, $sp, 4	
	
	jr	$ra

##############################################################################
#
#  DESCRIPTION : Prints out 'str = ' followed by the input string surronded
#		 by double quotes to the console. 
#
#        INPUT: $a0 - address to a NUL terminated string.
#
##############################################################################
print_test_string:	

	.data
STR_str_is:
	.asciiz "str = \""
STR_quote:
	.asciiz "\""	

	.text

	add	$t0, $a0, $zero
	
	li	$v0, 4
	la	$a0, STR_str_is
	syscall

	add	$a0, $t0, $zero
	syscall

	li	$v0, 4	
	la	$a0, STR_quote
	syscall
	
	jr	$ra
	

##############################################################################
#
#  DESCRIPTION: Prints out the Ascii value of a character.
#	
#        INPUT: $a0 - address of a character 
#
##############################################################################
ascii:	
	.data
STR_the_ascii_value_is:
	.asciiz "\nAscii('X') = "

	.text

	la	$t0, STR_the_ascii_value_is

	# Replace X with the input character
	
	add	$t1, $t0, 8	# Position of X
	lb	$t2, 0($a0)	# Get the Ascii value
	sb	$t2, 0($t1)

	# Print "The Ascii value of..."
	
	add	$a0, $t0, $zero 
	li	$v0, 4
	syscall

	# Append the Ascii value
	
	add	$a0, $t2, $zero
	li	$v0, 1
	syscall


	jr	$ra
	
