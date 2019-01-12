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
lw $t5, -56($fp)
move $a0, $t5
sw $ra, -60($fp)
sw $fp, -64($fp)
addi $fp, $fp, -60
jal write
lw $fp, -4($fp)
lw $ra, 0($fp)
li $t6, 0
sw $t6, -60($fp)
lw $t7, -60($fp)
move $v0, $t7
move $v1, $ra
lw $ra, 0($fp)
lw $fp, -4($fp)
jr $v1
