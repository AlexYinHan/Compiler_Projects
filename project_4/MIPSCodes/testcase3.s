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
lw $t3, -48($fp)
addi $t2, $t3, 0
sw $t2, -52($fp)
lw $t5, -52($fp)
lw $t4, 0($t5)
sw $t4, -56($fp)
move $t6, $a0
sw $t6, -60($fp)
lw $t1, -60($fp)
addi $t0, $t1, 4
sw $t0, -64($fp)
lw $t3, -64($fp)
lw $t2, 0($t3)
sw $t2, -68($fp)
lw $t5, -56($fp)
lw $t6, -68($fp)
add $t4, $t5, $t6
sw $t4, -72($fp)
lw $t7, -72($fp)
move $v0, $t7
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
main:
sw $ra, 0($sp)
sw $fp, -4($sp)
move $fp, $sp
li $t1, 1
sw $t1, -56($fp)
addi $t2, $fp, -52
sw $t2, -60($fp)
lw $t4, -60($fp)
addi $t3, $t4, 0
sw $t3, -64($fp)
lw $t5, -64($fp)
lw $t6, -56($fp)
sw $t6, 0($t5)
sw $t5, -64($fp)
li $t7, 2
sw $t7, -68($fp)
addi $t0, $fp, -52
sw $t0, -72($fp)
lw $t2, -72($fp)
addi $t1, $t2, 4
sw $t1, -76($fp)
lw $t3, -76($fp)
lw $t4, -68($fp)
sw $t4, 0($t3)
sw $t3, -76($fp)
addi $t5, $fp, -52
sw $t5, -80($fp)
lw $t6, -80($fp)
move $a0, $t6
sw $ra, -84($fp)
sw $fp, -88($fp)
sw $t0, -92($fp)
sw $t1, -96($fp)
sw $t2, -100($fp)
sw $t3, -104($fp)
sw $t4, -108($fp)
sw $t5, -112($fp)
sw $t6, -116($fp)
sw $t7, -120($fp)
sw $t8, -124($fp)
sw $t9, -128($fp)
add $fp, $fp, -84
jal adds
lw $t0, -92($fp)
lw $t1, -96($fp)
lw $t2, -100($fp)
lw $t3, -104($fp)
lw $t4, -108($fp)
lw $t5, -112($fp)
lw $t6, -116($fp)
lw $t7, -120($fp)
lw $t8, -124($fp)
lw $t9, -128($fp)
move $t7, $v0
sw $v0, -88($fp)
lw $t1, -88($fp)
move $t0, $t1
sw $t0, -92($fp)
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
