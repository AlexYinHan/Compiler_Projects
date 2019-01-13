.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
jr $ra

write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra

adds:
move $t0, $a0
sw $t0, -48($fp)
move $t2, $a1
sw $t2, -52($fp)
lw $t5, -48($fp)
lw $t6, -52($fp)
add $t4, $t5, $t6
sw $t4, -56($fp)
move $t7, $a2
sw $t7, -60($fp)
lw $t2, -56($fp)
lw $t3, -60($fp)
add $t1, $t2, $t3
sw $t1, -64($fp)
move $t4, $a3
sw $t4, -68($fp)
lw $t7, -64($fp)
lw $t0, -68($fp)
add $t6, $t7, $t0
sw $t6, -72($fp)
lw $t2, 4($fp)
move $t1, $t2
sw $t1, -76($fp)
lw $t4, -72($fp)
lw $t5, -76($fp)
add $t3, $t4, $t5
sw $t3, -80($fp)
lw $t7, 8($fp)
move $t6, $t7
sw $t6, -84($fp)
lw $t1, -80($fp)
lw $t2, -84($fp)
add $t0, $t1, $t2
sw $t0, -88($fp)
lw $t4, 12($fp)
move $t3, $t4
sw $t3, -92($fp)
lw $t6, -88($fp)
lw $t7, -92($fp)
add $t5, $t6, $t7
sw $t5, -96($fp)
lw $t1, -96($fp)
move $t0, $t1
sw $t0, -100($fp)
lw $t2, -100($fp)
move $v0, $t2
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
main:
sw $ra, 0($sp)
sw $fp, -4($sp)
move $fp, $sp
li $t3, 1
sw $t3, -48($fp)
li $t4, 2
sw $t4, -52($fp)
li $t5, 3
sw $t5, -56($fp)
li $t6, 4
sw $t6, -60($fp)
li $t7, 5
sw $t7, -64($fp)
li $t0, 6
sw $t0, -68($fp)
li $t1, 7
sw $t1, -72($fp)
lw $t2, -72($fp)
move $v1, $t2
sw $v1, -76($fp)
lw $t3, -68($fp)
move $v1, $t3
sw $v1, -80($fp)
lw $t4, -64($fp)
move $v1, $t4
sw $v1, -84($fp)
lw $t5, -60($fp)
move $a3, $t5
lw $t6, -56($fp)
move $a2, $t6
lw $t7, -52($fp)
move $a1, $t7
lw $t0, -48($fp)
move $a0, $t0
sw $ra, -88($fp)
sw $fp, -92($fp)
sw $t0, -96($fp)
sw $t1, -100($fp)
sw $t2, -104($fp)
sw $t3, -108($fp)
sw $t4, -112($fp)
sw $t5, -116($fp)
sw $t6, -120($fp)
sw $t7, -124($fp)
sw $t8, -128($fp)
sw $t9, -132($fp)
add $fp, $fp, -88
jal adds
lw $t0, -96($fp)
lw $t1, -100($fp)
lw $t2, -104($fp)
lw $t3, -108($fp)
lw $t4, -112($fp)
lw $t5, -116($fp)
lw $t6, -120($fp)
lw $t7, -124($fp)
lw $t8, -128($fp)
lw $t9, -132($fp)
move $t1, $v0
sw $v0, -92($fp)
lw $t3, -92($fp)
move $t2, $t3
sw $t2, -96($fp)
lw $t4, -96($fp)
move $a0, $t4
sw $ra, -100($fp)
sw $fp, -104($fp)
addi $fp, $fp, -100
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
li $t5, 0
sw $t5, -100($fp)
lw $t6, -100($fp)
move $v0, $t6
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
