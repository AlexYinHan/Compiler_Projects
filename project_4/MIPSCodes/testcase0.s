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
li $t0, 0
sw $t0, -48($fp)
li $t1, 1
sw $t1, -52($fp)
li $t2, 0
sw $t2, -56($fp)
sw $ra, -64($fp)
sw $fp, -68($fp)
addi $fp, $fp, -64
jal read
lw $fp, -4($fp)
lw $ra, 0($fp)
move $t3, $v0
sw $v0, -60($fp)
lw $t5, -60($fp)
move $t4, $t5
sw $t4, -64($fp)
label1:
lw $t7, -56($fp)
move $t6, $t7
sw $t6, -68($fp)
lw $t1, -64($fp)
move $t0, $t1
sw $t0, -72($fp)
lw $t2, -68($fp)
lw $t3, -72($fp)
blt $t2, $t3, label2
j label3
label2:
lw $t5, -48($fp)
move $t4, $t5
sw $t4, -76($fp)
lw $t7, -52($fp)
move $t6, $t7
sw $t6, -80($fp)
lw $t1, -76($fp)
lw $t2, -80($fp)
add $t0, $t1, $t2
sw $t0, -84($fp)
lw $t4, -52($fp)
move $t3, $t4
sw $t3, -88($fp)
lw $t5, -88($fp)
move $a0, $t5
sw $ra, -92($fp)
sw $fp, -96($fp)
addi $fp, $fp, -92
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
lw $t7, -52($fp)
move $t6, $t7
sw $t6, -92($fp)
lw $t0, -48($fp)
lw $t1, -92($fp)
move $t0, $t1
sw $t0, -48($fp)
lw $t3, -84($fp)
move $t2, $t3
sw $t2, -96($fp)
lw $t4, -52($fp)
lw $t5, -96($fp)
move $t4, $t5
sw $t4, -52($fp)
lw $t7, -56($fp)
move $t6, $t7
sw $t6, -100($fp)
li $t0, 1
sw $t0, -104($fp)
lw $t2, -100($fp)
lw $t3, -104($fp)
add $t1, $t2, $t3
sw $t1, -108($fp)
lw $t4, -56($fp)
lw $t5, -108($fp)
move $t4, $t5
sw $t4, -56($fp)
j label1
label3:
li $t6, 0
sw $t6, -112($fp)
lw $t7, -112($fp)
move $v0, $t7
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
