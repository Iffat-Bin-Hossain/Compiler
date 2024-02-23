	; line 1
.model small
.stack 1000h
.Data
	number DB "00000$"
	 DW 1 DUP (0000H)
	 DW 1 DUP (0000H)
	 DW 1 DUP (0000H)
.code
Print proc
	; Preserve AX,BX,CX,DX register
	PUSH ax
	PUSH bx
	PUSH cx
	PUSH dx
	; quotient in ax, remainder in dx
	; divisor in cx=10
	; the length of the number in bl
	; the sign bit in bh(1 for negative number)
	MOV cx, 10
	XOR bl, bl
	CMP ax, 0
	JGE stackPush   ; for positive number
	MOV bh, 1
	NEG ax          ; convert to positive
stackPush:
	; This procedure will continue untill ax=0
	XOR dx, dx
	div cx
	push dx         ; Push remainder in stack
	INC bl          ; Increment length
	CMP ax, 0
	JG stackPush    ; continue division
	MOV ah, 02      ; display
	CMP bh, 1       ; Check if number is negative
	JNE stackPop    ; if not negative,start printing
	MOV dl, '-'     ; If negative, print a '-' sign first
	int 21h
stackPop:
	POP dx          ; Pop remainder from stack
	XOR dh, dh
	ADD dl, '0'     ; Convert remainder to ASCII
	int 21h
	DEC bl          ; Decrement length
	CMP bl, 0
	JG stackPop     ; more to print
	; Cleanup AX,BX,CX,DX registers
	POP dx
	POP cx
	POP bx
	POP ax
	RET
Print endp
Newline proc
	PUSH AX
	PUSH DX
	MOV AH, 02
	MOV DL, 0DH
	int 21H
	MOV DL, 0AH
	int 21H
	POP DX
	POP AX
	RET
Newline ENDP
	SUB SP, 2
	; line 2
main PROC
	MOV AX, @DATA
	MOV DS, AX
	PUSH BP
	MOV BP, SP
	; line 4
	SUB SP, 2
	SUB SP, 2
	SUB SP, 2
	SUB SP, 2
	SUB SP, 2
	SUB SP, 2
	; line 6
	MOV AX, 1
	MOV [BP+0], AX
	; line 7
	CALL Print
	CALL Newline
	; line 9
	MOV AX, 8
	PUSH AX
	MOV AX, 5
	POP CX
	ADD AX, CX
	MOV [BP+0], AX
	; line 10
	CALL Print
	CALL Newline
	; line 12
	MOV AX, [BP+0]
	PUSH AX
	MOV AX, 2
	POP CX
	CWD
	MUL CX
	PUSH AX
	MOV AX, [BP+0]
	POP CX
	ADD AX, CX
	MOV [BP+0], AX
	; line 13
	CALL Print
	CALL Newline
	; line 15
	MOV AX, 9
	PUSH AX
	MOV AX, [BP+0]
	POP CX
	CWD
	DIV CX
	MOV AX, DX
	MOV [BP+0], AX
	; line 16
	CALL Print
	CALL Newline
	; line 18
	MOV AX, [BP+0]
	PUSH AX
	MOV AX, [BP+0]
	POP DX
	CMP DX,AX
	JLE L1
	JMP L2
L1:
	MOV AX, 1
	JMP L3
L2:
	MOV AX, 0
L3:
	MOV [BP+0], AX
	; line 19
	CALL Print
	CALL Newline
	; line 21
	MOV AX, [BP+0]
	PUSH AX
	MOV AX, [BP+0]
	POP DX
	CMP DX,AX
	JNE L4
	JMP L5
L4:
	MOV AX, 1
	JMP L6
L5:
	MOV AX, 0
L6:
	MOV [BP+0], AX
	; line 22
	CALL Print
	CALL Newline
	; line 24
	MOV AX, [BP+0]
	CMP AX,0
	JNE L7
	MOV AX, [BP+0]
	CMP AX,0
	JNE L7
	MOV AX,0
	JMP L8
L7:
	MOV AX,1
L8:
	MOV [BP+0], AX
	; line 25
	CALL Print
	CALL Newline
	; line 27
	MOV AX, [BP+0]
	CMP AX,0
	JE L9
	MOV AX, [BP+0]
	CMP AX,0
	JE L9
	MOV AX,1
	JMP L10
L9:
	MOV AX,0
L10:
	MOV [BP+0], AX
	; line 28
	CALL Print
	CALL Newline
	; line 30
	MOV AX, [BP+0]
	PUSH AX
	DEC AX
	MOV [BP+0], AX
	POP AX
	; line 31
	CALL Print
	CALL Newline
	; line 33
	MOV AX, [BP+0]
	NEG AX
	MOV [BP+0], AX
	; line 34
	CALL Print
	CALL Newline
	; line 36
	MOV AX, 0
	JMP L11
	; line 37
L11:
	ADD SP, 12
	POP BP
	MOV AX, 4CH
	INT 21H
main ENDP
END main
