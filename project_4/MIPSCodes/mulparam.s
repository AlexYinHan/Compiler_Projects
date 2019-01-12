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

max:
li $t0, 0
sw $t0, -48($fp)
li $t1, 0
sw $t1, -52($fp)
li $t2, 0
sw $t2, -56($fp)
lw $t4, -56($fp)
li $v1, 8
mul $t3, $t4, $v1
sw $t3, -60($fp)
move $t5, $a0
sw $t5, -64($fp)
lw $t7, -64($fp)
lw $t0, -64($fp)
lw $t1, -60($fp)
add $t7, $t0, $t1
sw $t7, -64($fp)
li $t2, 0
sw $t2, -68($fp)
lw $t4, -68($fp)
li $v1, 4
mul $t3, $t4, $v1
sw $t3, -72($fp)
lw $t6, -64($fp)
move $t5, $t6
sw $t5, -76($fp)
lw $t7, -76($fp)
lw $t0, -76($fp)
lw $t1, -72($fp)
add $t7, $t0, $t1
sw $t7, -76($fp)
lw $t3, -76($fp)
lw $t2, 0($t3)
sw $t2, -80($fp)
label1:
lw $t5, -48($fp)
move $t4, $t5
sw $t4, -84($fp)
li $t6, 2
sw $t6, -88($fp)
lw $t7, -84($fp)
lw $t0, -88($fp)
blt $t7, $t0, label2
j label3
label2:
li $t1, 0
sw $t1, -92($fp)
lw $t2, -52($fp)
lw $t3, -92($fp)
move $t2, $t3
sw $t2, -52($fp)
label4:
lw $t5, -52($fp)
move $t4, $t5
sw $t4, -96($fp)
li $t6, 2
sw $t6, -100($fp)
lw $t7, -96($fp)
lw $t0, -100($fp)
blt $t7, $t0, label5
j label6
label5:
lw $t2, -48($fp)
move $t1, $t2
sw $t1, -104($fp)
lw $t4, -104($fp)
li $v1, 8
mul $t3, $t4, $v1
sw $t3, -108($fp)
move $t5, $a0
sw $t5, -112($fp)
lw $t7, -112($fp)
lw $t0, -112($fp)
lw $t1, -108($fp)
add $t7, $t0, $t1
sw $t7, -112($fp)
lw $t3, -52($fp)
move $t2, $t3
sw $t2, -116($fp)
lw $t5, -116($fp)
li $v1, 4
mul $t4, $t5, $v1
sw $t4, -120($fp)
lw $t7, -112($fp)
move $t6, $t7
sw $t6, -124($fp)
lw $t0, -124($fp)
lw $t1, -124($fp)
lw $t2, -120($fp)
add $t0, $t1, $t2
sw $t0, -124($fp)
lw $t4, -124($fp)
lw $t3, 0($t4)
sw $t3, -128($fp)
lw $t6, -80($fp)
move $t5, $t6
sw $t5, -132($fp)
lw $t7, -128($fp)
lw $t0, -132($fp)
bgt $t7, $t0, label7
j label8
label7:
lw $t2, -48($fp)
move $t1, $t2
sw $t1, -136($fp)
lw $t4, -136($fp)
li $v1, 8
mul $t3, $t4, $v1
sw $t3, -140($fp)
move $t5, $a0
sw $t5, -144($fp)
lw $t7, -144($fp)
lw $t0, -144($fp)
lw $t1, -140($fp)
add $t7, $t0, $t1
sw $t7, -144($fp)
lw $t3, -52($fp)
move $t2, $t3
sw $t2, -148($fp)
lw $t5, -148($fp)
li $v1, 4
mul $t4, $t5, $v1
sw $t4, -152($fp)
lw $t7, -144($fp)
move $t6, $t7
sw $t6, -156($fp)
lw $t0, -156($fp)
lw $t1, -156($fp)
lw $t2, -152($fp)
add $t0, $t1, $t2
sw $t0, -156($fp)
lw $t4, -156($fp)
lw $t3, 0($t4)
sw $t3, -160($fp)
lw $t5, -80($fp)
lw $t6, -160($fp)
move $t5, $t6
sw $t5, -80($fp)
label8:
lw $t0, -52($fp)
move $t7, $t0
sw $t7, -164($fp)
li $t1, 1
sw $t1, -168($fp)
lw $t3, -164($fp)
lw $t4, -168($fp)
add $t2, $t3, $t4
sw $t2, -172($fp)
lw $t5, -52($fp)
lw $t6, -172($fp)
move $t5, $t6
sw $t5, -52($fp)
j label4
label6:
lw $t0, -48($fp)
move $t7, $t0
sw $t7, -176($fp)
li $t1, 1
sw $t1, -180($fp)
lw $t3, -176($fp)
lw $t4, -180($fp)
add $t2, $t3, $t4
sw $t2, -184($fp)
lw $t5, -48($fp)
lw $t6, -184($fp)
move $t5, $t6
sw $t5, -48($fp)
j label1
label3:
lw $t0, -80($fp)
move $t7, $t0
sw $t7, -188($fp)
lw $t1, -188($fp)
move $v0, $t1
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
main:
sw $ra, 0($sp)
sw $fp, -4($sp)
move $fp, $sp
li $t3, 1
sw $t3, -68($fp)
addi $t4, $fp, -64
sw $t4, -72($fp)
lw $t6, -72($fp)
addi $t5, $t6, 4
sw $t5, -76($fp)
li $t7, 0
sw $t7, -80($fp)
lw $t1, -80($fp)
li $v1, 8
mul $t0, $t1, $v1
sw $t0, -84($fp)
lw $t3, -76($fp)
move $t2, $t3
sw $t2, -88($fp)
lw $t4, -88($fp)
lw $t5, -88($fp)
lw $t6, -84($fp)
add $t4, $t5, $t6
sw $t4, -88($fp)
li $t7, 0
sw $t7, -92($fp)
lw $t1, -92($fp)
li $v1, 4
mul $t0, $t1, $v1
sw $t0, -96($fp)
lw $t3, -88($fp)
move $t2, $t3
sw $t2, -100($fp)
lw $t4, -100($fp)
lw $t5, -100($fp)
lw $t6, -96($fp)
add $t4, $t5, $t6
sw $t4, -100($fp)
lw $t7, -100($fp)
lw $t0, -68($fp)
sw $t0, 0($t7)
sw $t7, -100($fp)
li $t1, 2
sw $t1, -104($fp)
addi $t2, $fp, -64
sw $t2, -108($fp)
lw $t4, -108($fp)
addi $t3, $t4, 4
sw $t3, -112($fp)
li $t5, 0
sw $t5, -116($fp)
lw $t7, -116($fp)
li $v1, 8
mul $t6, $t7, $v1
sw $t6, -120($fp)
lw $t1, -112($fp)
move $t0, $t1
sw $t0, -124($fp)
lw $t2, -124($fp)
lw $t3, -124($fp)
lw $t4, -120($fp)
add $t2, $t3, $t4
sw $t2, -124($fp)
li $t5, 1
sw $t5, -128($fp)
lw $t7, -128($fp)
li $v1, 4
mul $t6, $t7, $v1
sw $t6, -132($fp)
lw $t1, -124($fp)
move $t0, $t1
sw $t0, -136($fp)
lw $t2, -136($fp)
lw $t3, -136($fp)
lw $t4, -132($fp)
add $t2, $t3, $t4
sw $t2, -136($fp)
lw $t5, -136($fp)
lw $t6, -104($fp)
sw $t6, 0($t5)
sw $t5, -136($fp)
li $t7, 3
sw $t7, -140($fp)
addi $t0, $fp, -64
sw $t0, -144($fp)
lw $t2, -144($fp)
addi $t1, $t2, 4
sw $t1, -148($fp)
li $t3, 1
sw $t3, -152($fp)
lw $t5, -152($fp)
li $v1, 8
mul $t4, $t5, $v1
sw $t4, -156($fp)
lw $t7, -148($fp)
move $t6, $t7
sw $t6, -160($fp)
lw $t0, -160($fp)
lw $t1, -160($fp)
lw $t2, -156($fp)
add $t0, $t1, $t2
sw $t0, -160($fp)
li $t3, 0
sw $t3, -164($fp)
lw $t5, -164($fp)
li $v1, 4
mul $t4, $t5, $v1
sw $t4, -168($fp)
lw $t7, -160($fp)
move $t6, $t7
sw $t6, -172($fp)
lw $t0, -172($fp)
lw $t1, -172($fp)
lw $t2, -168($fp)
add $t0, $t1, $t2
sw $t0, -172($fp)
lw $t3, -172($fp)
lw $t4, -140($fp)
sw $t4, 0($t3)
sw $t3, -172($fp)
li $t5, 4
sw $t5, -176($fp)
addi $t6, $fp, -64
sw $t6, -180($fp)
lw $t0, -180($fp)
addi $t7, $t0, 4
sw $t7, -184($fp)
li $t1, 1
sw $t1, -188($fp)
lw $t3, -188($fp)
li $v1, 8
mul $t2, $t3, $v1
sw $t2, -192($fp)
lw $t5, -184($fp)
move $t4, $t5
sw $t4, -196($fp)
lw $t6, -196($fp)
lw $t7, -196($fp)
lw $t0, -192($fp)
add $t6, $t7, $t0
sw $t6, -196($fp)
li $t1, 1
sw $t1, -200($fp)
lw $t3, -200($fp)
li $v1, 4
mul $t2, $t3, $v1
sw $t2, -204($fp)
lw $t5, -196($fp)
move $t4, $t5
sw $t4, -208($fp)
lw $t6, -208($fp)
lw $t7, -208($fp)
lw $t0, -204($fp)
add $t6, $t7, $t0
sw $t6, -208($fp)
lw $t1, -208($fp)
lw $t2, -176($fp)
sw $t2, 0($t1)
sw $t1, -208($fp)
addi $t3, $fp, -64
sw $t3, -212($fp)
lw $t5, -212($fp)
addi $t4, $t5, 4
sw $t4, -216($fp)
lw $t7, -216($fp)
move $t6, $t7
sw $t6, -220($fp)
lw $t0, -220($fp)
move $a0, $t0
sw $ra, -224($fp)
sw $fp, -228($fp)
sw $t0, -232($fp)
sw $t1, -236($fp)
sw $t2, -240($fp)
sw $t3, -244($fp)
sw $t4, -248($fp)
sw $t5, -252($fp)
sw $t6, -256($fp)
sw $t7, -260($fp)
sw $t8, -264($fp)
sw $t9, -268($fp)
add $fp, $fp, -224
jal max
lw $t0, -232($fp)
lw $t1, -236($fp)
lw $t2, -240($fp)
lw $t3, -244($fp)
lw $t4, -248($fp)
lw $t5, -252($fp)
lw $t6, -256($fp)
lw $t7, -260($fp)
lw $t8, -264($fp)
lw $t9, -268($fp)
move $t1, $v0
sw $v0, -228($fp)
lw $t3, -228($fp)
move $t2, $t3
sw $t2, -232($fp)
lw $t5, -232($fp)
move $t4, $t5
sw $t4, -236($fp)
lw $t6, -236($fp)
move $a0, $t6
sw $ra, -240($fp)
sw $fp, -244($fp)
addi $fp, $fp, -240
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
li $t7, 0
sw $t7, -240($fp)
lw $t0, -240($fp)
move $v0, $t0
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
