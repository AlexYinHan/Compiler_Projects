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
li $t2, 0
sw $t2, -64($fp)
li $t3, 0
sw $t3, -68($fp)
label1:
lw $t5, -64($fp)
move $t4, $t5
sw $t4, -72($fp)
li $t6, 2
sw $t6, -76($fp)
lw $t7, -72($fp)
lw $t0, -76($fp)
blt $t7, $t0, label2
j label3
label2:
label4:
lw $t2, -68($fp)
move $t1, $t2
sw $t1, -80($fp)
li $t3, 2
sw $t3, -84($fp)
lw $t4, -80($fp)
lw $t5, -84($fp)
blt $t4, $t5, label5
j label6
label5:
lw $t7, -64($fp)
move $t6, $t7
sw $t6, -88($fp)
lw $t1, -68($fp)
move $t0, $t1
sw $t0, -92($fp)
lw $t3, -88($fp)
lw $t4, -92($fp)
add $t2, $t3, $t4
sw $t2, -96($fp)
lw $t6, -68($fp)
move $t5, $t6
sw $t5, -100($fp)
lw $t0, -100($fp)
li $v1, 4
mul $t7, $t0, $v1
sw $t7, -104($fp)
addi $t1, $fp, -52
sw $t1, -108($fp)
lw $t2, -108($fp)
lw $t3, -108($fp)
lw $t4, -104($fp)
add $t2, $t3, $t4
sw $t2, -108($fp)
lw $t5, -108($fp)
lw $t6, -96($fp)
sw $t6, 0($t5)
sw $t5, -108($fp)
lw $t0, -68($fp)
move $t7, $t0
sw $t7, -112($fp)
li $t1, 1
sw $t1, -116($fp)
lw $t3, -112($fp)
lw $t4, -116($fp)
add $t2, $t3, $t4
sw $t2, -120($fp)
lw $t5, -68($fp)
lw $t6, -120($fp)
move $t5, $t6
sw $t5, -68($fp)
j label4
label6:
addi $t7, $fp, -52
sw $t7, -124($fp)
lw $t0, -124($fp)
move $a0, $t0
sw $ra, -128($fp)
sw $fp, -132($fp)
sw $t0, -136($fp)
sw $t1, -140($fp)
sw $t2, -144($fp)
sw $t3, -148($fp)
sw $t4, -152($fp)
sw $t5, -156($fp)
sw $t6, -160($fp)
sw $t7, -164($fp)
sw $t8, -168($fp)
sw $t9, -172($fp)
add $fp, $fp, -128
jal adds
lw $t0, -136($fp)
lw $t1, -140($fp)
lw $t2, -144($fp)
lw $t3, -148($fp)
lw $t4, -152($fp)
lw $t5, -156($fp)
lw $t6, -160($fp)
lw $t7, -164($fp)
lw $t8, -168($fp)
lw $t9, -172($fp)
move $t1, $v0
sw $v0, -132($fp)
li $t2, 0
sw $t2, -136($fp)
lw $t4, -136($fp)
li $v1, 8
mul $t3, $t4, $v1
sw $t3, -140($fp)
addi $t5, $fp, -60
sw $t5, -144($fp)
lw $t6, -144($fp)
lw $t7, -144($fp)
lw $t0, -140($fp)
add $t6, $t7, $t0
sw $t6, -144($fp)
lw $t2, -64($fp)
move $t1, $t2
sw $t1, -148($fp)
lw $t4, -148($fp)
li $v1, 4
mul $t3, $t4, $v1
sw $t3, -152($fp)
lw $t6, -144($fp)
move $t5, $t6
sw $t5, -156($fp)
lw $t7, -156($fp)
lw $t0, -156($fp)
lw $t1, -152($fp)
add $t7, $t0, $t1
sw $t7, -156($fp)
lw $t2, -156($fp)
lw $t3, -132($fp)
sw $t3, 0($t2)
sw $t2, -156($fp)
li $t4, 0
sw $t4, -160($fp)
lw $t6, -160($fp)
li $v1, 8
mul $t5, $t6, $v1
sw $t5, -164($fp)
addi $t7, $fp, -60
sw $t7, -168($fp)
lw $t0, -168($fp)
lw $t1, -168($fp)
lw $t2, -164($fp)
add $t0, $t1, $t2
sw $t0, -168($fp)
lw $t4, -64($fp)
move $t3, $t4
sw $t3, -172($fp)
lw $t6, -172($fp)
li $v1, 4
mul $t5, $t6, $v1
sw $t5, -176($fp)
lw $t0, -168($fp)
move $t7, $t0
sw $t7, -180($fp)
lw $t1, -180($fp)
lw $t2, -180($fp)
lw $t3, -176($fp)
add $t1, $t2, $t3
sw $t1, -180($fp)
lw $t5, -180($fp)
lw $t4, 0($t5)
sw $t4, -184($fp)
lw $t6, -184($fp)
move $a0, $t6
sw $ra, -188($fp)
sw $fp, -192($fp)
addi $fp, $fp, -188
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
lw $t0, -64($fp)
move $t7, $t0
sw $t7, -188($fp)
li $t1, 1
sw $t1, -192($fp)
lw $t3, -188($fp)
lw $t4, -192($fp)
add $t2, $t3, $t4
sw $t2, -196($fp)
lw $t5, -64($fp)
lw $t6, -196($fp)
move $t5, $t6
sw $t5, -64($fp)
li $t7, 0
sw $t7, -200($fp)
lw $t0, -68($fp)
lw $t1, -200($fp)
move $t0, $t1
sw $t0, -68($fp)
j label1
label3:
li $t2, 0
sw $t2, -204($fp)
lw $t3, -204($fp)
move $v0, $t3
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
