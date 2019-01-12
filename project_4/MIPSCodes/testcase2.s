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

fact:
move $t0, $a0
sw $t0, -48($fp)
li $t2, 1
sw $t2, -52($fp)
lw $t3, -48($fp)
lw $t4, -52($fp)
beq $t3, $t4, label1
j label2
label1:
move $t5, $a0
sw $t5, -56($fp)
lw $t7, -56($fp)
move $v0, $t7
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
j label3
label2:
move $t0, $a0
sw $t0, -60($fp)
move $t2, $a0
sw $t2, -64($fp)
li $t4, 1
sw $t4, -68($fp)
lw $t6, -64($fp)
lw $t7, -68($fp)
sub $t5, $t6, $t7
sw $t5, -72($fp)
lw $t0, -72($fp)
move $a0, $t0
sw $ra, -76($fp)
sw $fp, -80($fp)
sw $t0, -84($fp)
sw $t1, -88($fp)
sw $t2, -92($fp)
sw $t3, -96($fp)
sw $t4, -100($fp)
sw $t5, -104($fp)
sw $t6, -108($fp)
sw $t7, -112($fp)
sw $t8, -116($fp)
sw $t9, -120($fp)
add $fp, $fp, -76
jal fact
lw $t0, -84($fp)
lw $t1, -88($fp)
lw $t2, -92($fp)
lw $t3, -96($fp)
lw $t4, -100($fp)
lw $t5, -104($fp)
lw $t6, -108($fp)
lw $t7, -112($fp)
lw $t8, -116($fp)
lw $t9, -120($fp)
move $t1, $v0
sw $v0, -80($fp)
lw $t3, -60($fp)
lw $t4, -80($fp)
mul $t2, $t3, $t4
sw $t2, -84($fp)
lw $t5, -84($fp)
move $v0, $t5
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
label3:
main:
sw $ra, 0($sp)
sw $fp, -4($sp)
move $fp, $sp
sw $ra, -52($fp)
sw $fp, -56($fp)
addi $fp, $fp, -52
jal read
lw $fp, -4($fp)
lw $ra, 0($fp)
move $t6, $v0
sw $v0, -48($fp)
lw $t0, -48($fp)
move $t7, $t0
sw $t7, -52($fp)
lw $t2, -52($fp)
move $t1, $t2
sw $t1, -56($fp)
li $t3, 1
sw $t3, -60($fp)
lw $t4, -56($fp)
lw $t5, -60($fp)
bgt $t4, $t5, label4
j label5
label4:
lw $t7, -52($fp)
move $t6, $t7
sw $t6, -64($fp)
lw $t0, -64($fp)
move $a0, $t0
sw $ra, -68($fp)
sw $fp, -72($fp)
sw $t0, -76($fp)
sw $t1, -80($fp)
sw $t2, -84($fp)
sw $t3, -88($fp)
sw $t4, -92($fp)
sw $t5, -96($fp)
sw $t6, -100($fp)
sw $t7, -104($fp)
sw $t8, -108($fp)
sw $t9, -112($fp)
add $fp, $fp, -68
jal fact
lw $t0, -76($fp)
lw $t1, -80($fp)
lw $t2, -84($fp)
lw $t3, -88($fp)
lw $t4, -92($fp)
lw $t5, -96($fp)
lw $t6, -100($fp)
lw $t7, -104($fp)
lw $t8, -108($fp)
lw $t9, -112($fp)
move $t1, $v0
sw $v0, -72($fp)
lw $t3, -72($fp)
move $t2, $t3
sw $t2, -76($fp)
j label6
label5:
li $t4, 1
sw $t4, -80($fp)
lw $t5, -76($fp)
lw $t6, -80($fp)
move $t5, $t6
sw $t5, -76($fp)
label6:
lw $t0, -76($fp)
move $t7, $t0
sw $t7, -84($fp)
lw $t1, -84($fp)
move $a0, $t1
sw $ra, -88($fp)
sw $fp, -92($fp)
addi $fp, $fp, -88
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
li $t2, 0
sw $t2, -88($fp)
lw $t3, -88($fp)
move $v0, $t3
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
