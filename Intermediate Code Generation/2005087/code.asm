.model small
.stack 1000h
.data


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
Main proc
	MOV ax,@data
	MOV ds,ax
	PUSH BP
	MOV BP,SP
L1:
MOV ax,1
PUSH ax
	POP ax
	MOV i,ax
POP ax
L2:
	PUSH ax
	MOV ax,i
	CALL Print
	CALL Newline
	POP ax
L3:
MOV ax,5
PUSH ax
MOV ax,8
PUSH ax
	POP bx
	POP ax
	ADD ax,bx
	PUSH ax
	POP ax
	MOV j,ax
POP ax
L4:
	PUSH ax
	MOV ax,j
	CALL Print
	CALL Newline
	POP ax
L5:
	MOV ax,i
	PUSH ax
MOV ax,2
PUSH ax
	MOV ax,j
	PUSH ax
	POP bx
	POP ax
	MUL BX
	PUSH ax
	POP bx
	POP ax
	ADD ax,bx
	PUSH ax
	POP ax
	MOV k,ax
POP ax
L6:
	PUSH ax
	MOV ax,k
	CALL Print
	CALL Newline
	POP ax
L7:
	MOV ax,k
	PUSH ax
MOV ax,9
PUSH ax
	POP bx
	POP ax
	CWD
	DIV BX
	MOV AX,DX
	PUSH ax
	POP ax
	MOV m,ax
POP ax
L8:
	PUSH ax
	MOV ax,m
	CALL Print
	CALL Newline
	POP ax
L9:
	MOV ax,m
	PUSH ax
	MOV ax,ll
	PUSH ax
	POP bx
	POP ax
	CMP ax,bx
	JLE 
	JMP 
PUSH ax
	POP ax
	MOV n,ax
POP ax
L10:
	PUSH ax
	MOV ax,n
	CALL Print
	CALL Newline
	POP ax
L11:
	MOV ax,i
	PUSH ax
	MOV ax,j
	PUSH ax
	POP bx
	POP ax
	CMP ax,bx
	JNE 
	JMP 
PUSH ax
	POP ax
	MOV o,ax
POP ax
L12:
	PUSH ax
	MOV ax,o
	CALL Print
	CALL Newline
	POP ax
L13:
	MOV ax,n
	PUSH ax
	POP ax
	CMP AX, 0
	JNE
	JMP
	PUSH ax
L14:
	MOV ax,o
	PUSH ax
	POP ax
	CMP AX, 0
	JNE
	JMP
	PUSH ax
POP bx
	POP ax
	PUSH ax
	POP ax
	MOV p,ax
POP ax
L15:
	PUSH ax
	MOV ax,p
	CALL Print
	CALL Newline
	POP ax
L16:
	MOV ax,n
	PUSH ax
	POP ax
	CMP AX, 0
	JNE
	JMP
	PUSH ax
L17:
	MOV ax,o
	PUSH ax
	POP ax
	CMP AX, 0
	JNE
	JMP
	PUSH ax
POP bx
	POP ax
	PUSH ax
	POP ax
	MOV p,ax
POP ax
L18:
	PUSH ax
	MOV ax,p
	CALL Print
	CALL Newline
	POP ax
L19:
	MOV ax,p
PUSH ax
INC W .p
POP ax
L20:
	PUSH ax
	MOV ax,p
	CALL Print
	CALL Newline
	POP ax
L21:
	MOV ax,p
	PUSH ax
	POP ax
	NEG ax
	PUSH ax
	POP ax
	MOV k,ax
POP ax
L22:
	PUSH ax
	MOV ax,k
	CALL Print
	CALL Newline
	POP ax
L23:
MOV ax,0
PUSH ax
L24:
L25:
	MOV SP, BP
	POP BP

	Exit:
	MOV ax,4ch
	int 21h
Main ENDP
