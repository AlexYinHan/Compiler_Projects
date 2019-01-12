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
li $t0, 0
sw $t0, -48($fp)
lw $t2, -48($fp)
li $v1, 4
mul $t1, $t2, $v1
sw $t1, -52($fp)
move $t3, $a0
sw $t3, -56($fp)
lw $t5, -56($fp)
lw $t6, -56($fp)
lw $t7, -52($fp)
add $t5, $t6, $t7
sw $t5, -56($fp)
lw $t1, -56($fp)
lw $t0, 0($t1)
sw $t0, -60($fp)
li $t2, 1
sw $t2, -64($fp)
lw $t4, -64($fp)
li $v1, 4
mul $t3, $t4, $v1
sw $t3, -68($fp)
move $t5, $a0
sw $t5, -72($fp)
lw $t7, -72($fp)
lw $t0, -72($fp)
lw $t1, -68($fp)
add $t7, $t0, $t1
sw $t7, -72($fp)
lw $t3, -72($fp)
lw $t2, 0($t3)
sw $t2, -76($fp)
lw $t5, -60($fp)
lw $t6, -76($fp)
add $t4, $t5, $t6
sw $t4, -80($fp)
lw $t7, -80($fp)
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
li $t2, 0
sw $t2, -60($fp)
lw $t4, -60($fp)
li $v1, 4
mul $t3, $t4, $v1
sw $t3, -64($fp)
addi $t5, $fp, -52
sw $t5, -68($fp)
lw $t6, -68($fp)
lw $t7, -68($fp)
lw $t0, -64($fp)
add $t6, $t7, $t0
sw $t6, -68($fp)
lw $t1, -68($fp)
lw $t2, -56($fp)
sw $t2, 0($t1)
sw $t1, -68($fp)
li $t3, 2
sw $t3, -72($fp)
li $t4, 1
sw $t4, -76($fp)
lw $t6, -76($fp)
li $v1, 4
mul $t5, $t6, $v1
sw $t5, -80($fp)
addi $t7, $fp, -52
sw $t7, -84($fp)
lw $t0, -84($fp)
lw $t1, -84($fp)
lw $t2, -80($fp)
add $t0, $t1, $t2
sw $t0, -84($fp)
lw $t3, -84($fp)
lw $t4, -72($fp)
sw $t4, 0($t3)
sw $t3, -84($fp)
addi $t5, $fp, -52
sw $t5, -88($fp)
move $t6, $t7
sw $t6, -92($fp)
lw $t0, -92($fp)
move $a0, $t0
lw $t1, -88($fp)
move $a1, $t1
sw $ra, -100($fp)
sw $fp, -104($fp)
sw $t0, -108($fp)
sw $t1, -112($fp)
sw $t2, -116($fp)
sw $t3, -120($fp)
sw $t4, -124($fp)
sw $t5, -128($fp)
sw $t6, -132($fp)
sw $t7, -136($fp)
sw $t8, -140($fp)
sw $t9, -144($fp)
add $fp, $fp, -100
jal adds
lw $t0, -108($fp)
lw $t1, -112($fp)
lw $t2, -116($fp)
lw $t3, -120($fp)
lw $t4, -124($fp)
lw $t5, -128($fp)
lw $t6, -132($fp)
lw $t7, -136($fp)
lw $t8, -140($fp)
lw $t9, -144($fp)
move $t2, $v0
sw $v0, -104($fp)
lw $t3, -96($fp)
lw $t4, -104($fp)
move $t3, $t4
sw $t3, -96($fp)
lw $t6, -96($fp)
move $t5, $t6
sw $t5, -108($fp)
lw $t7, -108($fp)
move $a0, $t7
sw $ra, -112($fp)
sw $fp, -116($fp)
addi $fp, $fp, -112
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
li $t0, 0
sw $t0, -112($fp)
lw $t1, -112($fp)
move $v0, $t1
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
