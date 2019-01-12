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

sort:
li $t0, 0
sw $t0, -48($fp)
li $t1, 0
sw $t1, -52($fp)
li $t2, 0
sw $t2, -56($fp)
label1:
lw $t4, -48($fp)
move $t3, $t4
sw $t3, -60($fp)
li $t5, 3
sw $t5, -64($fp)
lw $t6, -60($fp)
lw $t7, -64($fp)
blt $t6, $t7, label2
j label3
label2:
lw $t1, -48($fp)
move $t0, $t1
sw $t0, -68($fp)
li $t2, 1
sw $t2, -72($fp)
lw $t4, -68($fp)
lw $t5, -72($fp)
add $t3, $t4, $t5
sw $t3, -76($fp)
lw $t6, -52($fp)
lw $t7, -76($fp)
move $t6, $t7
sw $t6, -52($fp)
label4:
lw $t1, -52($fp)
move $t0, $t1
sw $t0, -80($fp)
li $t2, 3
sw $t2, -84($fp)
lw $t3, -80($fp)
lw $t4, -84($fp)
blt $t3, $t4, label5
j label6
label5:
lw $t6, -48($fp)
move $t5, $t6
sw $t5, -88($fp)
lw $t0, -88($fp)
li $v1, 8
mul $t7, $t0, $v1
sw $t7, -92($fp)
move $t1, $a0
sw $t1, -96($fp)
lw $t3, -96($fp)
lw $t4, -96($fp)
lw $t5, -92($fp)
add $t3, $t4, $t5
sw $t3, -96($fp)
lw $t7, -96($fp)
move $t6, $t7
sw $t6, -100($fp)
lw $t1, -100($fp)
addi $t0, $t1, 0
sw $t0, -104($fp)
lw $t3, -104($fp)
lw $t2, 0($t3)
sw $t2, -108($fp)
lw $t5, -52($fp)
move $t4, $t5
sw $t4, -112($fp)
lw $t7, -112($fp)
li $v1, 8
mul $t6, $t7, $v1
sw $t6, -116($fp)
move $t0, $a0
sw $t0, -120($fp)
lw $t2, -120($fp)
lw $t3, -120($fp)
lw $t4, -116($fp)
add $t2, $t3, $t4
sw $t2, -120($fp)
lw $t6, -120($fp)
move $t5, $t6
sw $t5, -124($fp)
lw $t0, -124($fp)
addi $t7, $t0, 0
sw $t7, -128($fp)
lw $t2, -128($fp)
lw $t1, 0($t2)
sw $t1, -132($fp)
lw $t3, -108($fp)
lw $t4, -132($fp)
bgt $t3, $t4, label7
j label8
label7:
lw $t6, -48($fp)
move $t5, $t6
sw $t5, -136($fp)
lw $t0, -136($fp)
li $v1, 8
mul $t7, $t0, $v1
sw $t7, -140($fp)
move $t1, $a0
sw $t1, -144($fp)
lw $t3, -144($fp)
lw $t4, -144($fp)
lw $t5, -140($fp)
add $t3, $t4, $t5
sw $t3, -144($fp)
lw $t7, -144($fp)
move $t6, $t7
sw $t6, -148($fp)
lw $t1, -148($fp)
addi $t0, $t1, 0
sw $t0, -152($fp)
lw $t3, -152($fp)
lw $t2, 0($t3)
sw $t2, -156($fp)
lw $t4, -56($fp)
lw $t5, -156($fp)
move $t4, $t5
sw $t4, -56($fp)
lw $t7, -52($fp)
move $t6, $t7
sw $t6, -160($fp)
lw $t1, -160($fp)
li $v1, 8
mul $t0, $t1, $v1
sw $t0, -164($fp)
move $t2, $a0
sw $t2, -168($fp)
lw $t4, -168($fp)
lw $t5, -168($fp)
lw $t6, -164($fp)
add $t4, $t5, $t6
sw $t4, -168($fp)
lw $t0, -168($fp)
move $t7, $t0
sw $t7, -172($fp)
lw $t2, -172($fp)
addi $t1, $t2, 0
sw $t1, -176($fp)
lw $t4, -176($fp)
lw $t3, 0($t4)
sw $t3, -180($fp)
lw $t6, -48($fp)
move $t5, $t6
sw $t5, -184($fp)
lw $t0, -184($fp)
li $v1, 8
mul $t7, $t0, $v1
sw $t7, -188($fp)
move $t1, $a0
sw $t1, -192($fp)
lw $t3, -192($fp)
lw $t4, -192($fp)
lw $t5, -188($fp)
add $t3, $t4, $t5
sw $t3, -192($fp)
lw $t7, -192($fp)
move $t6, $t7
sw $t6, -196($fp)
lw $t1, -196($fp)
addi $t0, $t1, 0
sw $t0, -200($fp)
lw $t2, -200($fp)
lw $t3, -180($fp)
sw $t3, 0($t2)
sw $t2, -200($fp)
lw $t5, -56($fp)
move $t4, $t5
sw $t4, -204($fp)
lw $t7, -52($fp)
move $t6, $t7
sw $t6, -208($fp)
lw $t1, -208($fp)
li $v1, 8
mul $t0, $t1, $v1
sw $t0, -212($fp)
move $t2, $a0
sw $t2, -216($fp)
lw $t4, -216($fp)
lw $t5, -216($fp)
lw $t6, -212($fp)
add $t4, $t5, $t6
sw $t4, -216($fp)
lw $t0, -216($fp)
move $t7, $t0
sw $t7, -220($fp)
lw $t2, -220($fp)
addi $t1, $t2, 0
sw $t1, -224($fp)
lw $t3, -224($fp)
lw $t4, -204($fp)
sw $t4, 0($t3)
sw $t3, -224($fp)
label8:
lw $t6, -52($fp)
move $t5, $t6
sw $t5, -228($fp)
li $t7, 1
sw $t7, -232($fp)
lw $t1, -228($fp)
lw $t2, -232($fp)
add $t0, $t1, $t2
sw $t0, -236($fp)
lw $t3, -52($fp)
lw $t4, -236($fp)
move $t3, $t4
sw $t3, -52($fp)
j label4
label6:
lw $t6, -48($fp)
move $t5, $t6
sw $t5, -240($fp)
li $t7, 1
sw $t7, -244($fp)
lw $t1, -240($fp)
lw $t2, -244($fp)
add $t0, $t1, $t2
sw $t0, -248($fp)
lw $t3, -48($fp)
lw $t4, -248($fp)
move $t3, $t4
sw $t3, -48($fp)
j label1
label3:
li $t5, 0
sw $t5, -252($fp)
lw $t6, -252($fp)
move $v0, $t6
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
main:
sw $ra, 0($sp)
sw $fp, -4($sp)
move $fp, $sp
li $t0, 0
sw $t0, -72($fp)
li $t1, 3
sw $t1, -76($fp)
label9:
lw $t3, -72($fp)
move $t2, $t3
sw $t2, -80($fp)
lw $t5, -76($fp)
move $t4, $t5
sw $t4, -84($fp)
lw $t6, -80($fp)
lw $t7, -84($fp)
blt $t6, $t7, label10
j label11
label10:
sw $ra, -92($fp)
sw $fp, -96($fp)
addi $fp, $fp, -92
jal read
lw $fp, -4($fp)
lw $ra, 0($fp)
move $t0, $v0
sw $v0, -88($fp)
lw $t2, -72($fp)
move $t1, $t2
sw $t1, -92($fp)
lw $t4, -92($fp)
li $v1, 8
mul $t3, $t4, $v1
sw $t3, -96($fp)
addi $t5, $fp, -68
sw $t5, -100($fp)
lw $t6, -100($fp)
lw $t7, -100($fp)
lw $t0, -96($fp)
add $t6, $t7, $t0
sw $t6, -100($fp)
lw $t2, -100($fp)
move $t1, $t2
sw $t1, -104($fp)
lw $t4, -104($fp)
addi $t3, $t4, 0
sw $t3, -108($fp)
lw $t5, -108($fp)
lw $t6, -88($fp)
sw $t6, 0($t5)
sw $t5, -108($fp)
lw $t0, -72($fp)
move $t7, $t0
sw $t7, -112($fp)
li $t1, 1
sw $t1, -116($fp)
lw $t3, -112($fp)
lw $t4, -116($fp)
add $t2, $t3, $t4
sw $t2, -120($fp)
lw $t5, -72($fp)
lw $t6, -120($fp)
move $t5, $t6
sw $t5, -72($fp)
j label9
label11:
addi $t7, $fp, -68
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
jal sort
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
lw $t2, -72($fp)
lw $t3, -132($fp)
move $t2, $t3
sw $t2, -72($fp)
li $t4, 0
sw $t4, -136($fp)
lw $t5, -72($fp)
lw $t6, -136($fp)
move $t5, $t6
sw $t5, -72($fp)
label12:
lw $t0, -72($fp)
move $t7, $t0
sw $t7, -140($fp)
lw $t2, -76($fp)
move $t1, $t2
sw $t1, -144($fp)
lw $t3, -140($fp)
lw $t4, -144($fp)
blt $t3, $t4, label13
j label14
label13:
lw $t6, -72($fp)
move $t5, $t6
sw $t5, -148($fp)
lw $t0, -148($fp)
li $v1, 8
mul $t7, $t0, $v1
sw $t7, -152($fp)
addi $t1, $fp, -68
sw $t1, -156($fp)
lw $t2, -156($fp)
lw $t3, -156($fp)
lw $t4, -152($fp)
add $t2, $t3, $t4
sw $t2, -156($fp)
lw $t6, -156($fp)
move $t5, $t6
sw $t5, -160($fp)
lw $t0, -160($fp)
addi $t7, $t0, 0
sw $t7, -164($fp)
lw $t2, -164($fp)
lw $t1, 0($t2)
sw $t1, -168($fp)
lw $t3, -168($fp)
move $a0, $t3
sw $ra, -172($fp)
sw $fp, -176($fp)
addi $fp, $fp, -172
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
lw $t5, -72($fp)
move $t4, $t5
sw $t4, -172($fp)
li $t6, 1
sw $t6, -176($fp)
lw $t0, -172($fp)
lw $t1, -176($fp)
add $t7, $t0, $t1
sw $t7, -180($fp)
lw $t2, -72($fp)
lw $t3, -180($fp)
move $t2, $t3
sw $t2, -72($fp)
j label12
label14:
li $t4, 0
sw $t4, -184($fp)
lw $t5, -184($fp)
move $v0, $t5
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
