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
move $t0, $v0
sw $v0, -48($fp)
lw $t2, -48($fp)
move $t1, $t2
sw $t1, -52($fp)
lw $t4, -52($fp)
move $t3, $t4
sw $t3, -56($fp)
li $t5, 0
sw $t5, -60($fp)
lw $t6, -56($fp)
lw $t7, -60($fp)
bgt $t6, $t7, label1
j label2
label1:
li $t0, 1
sw $t0, -64($fp)
lw $t1, -64($fp)
move $a0, $t1
sw $ra, -68($fp)
sw $fp, -72($fp)
addi $fp, $fp, -68
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
j label3
label2:
lw $t3, -52($fp)
move $t2, $t3
sw $t2, -68($fp)
li $t4, 0
sw $t4, -72($fp)
lw $t5, -68($fp)
lw $t6, -72($fp)
blt $t5, $t6, label4
j label5
label4:
li $t7, 1
sw $t7, -76($fp)
lw $t1, -76($fp)
li $v1, 0
sub $t0, $v1, $t1
sw $t0, -80($fp)
lw $t2, -80($fp)
move $a0, $t2
sw $ra, -84($fp)
sw $fp, -88($fp)
addi $fp, $fp, -84
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
j label6
label5:
li $t3, 0
sw $t3, -84($fp)
lw $t4, -84($fp)
move $a0, $t4
sw $ra, -88($fp)
sw $fp, -92($fp)
addi $fp, $fp, -88
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
label6:
label3:
li $t5, 0
sw $t5, -88($fp)
lw $t6, -88($fp)
move $v0, $t6
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
