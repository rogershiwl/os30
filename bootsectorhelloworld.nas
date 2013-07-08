; boot sector header
; tab=4

    ORG 0x7c00      ; program load addr
    JMP entry
    
; standard FAT12 floppy format
;    DB 0xeb, 0x4c, 0x90
    DB 0x90
    DB "HELLOIPL"   ; boot sector name, 8 bytes
    DW 512          ; sector size
    DB 1            ; cluster size
    DW 1            ; FAT start position, sector
    DB 2            ; FAT num, must be 2
    DW 224          ; root dir size, normally 224
    DW 2880         ; disk size, sector
    DB 0xf0         ; disk type, must be 0xf0
    DW 9            ; FAT size, must be 9 sector
    DW 18           ; 1 track = 18 sector
    DW 2            ; head number, must be 2
    DD 0            ; not used sector, must be 0
    DD 2880         ; resure disk size
    DB 0, 0, 0x29   ; fixed
    DD 0xffffffff   ; volumn num
    DB "HELLO-OS   "; disk name, 11 bytes
    DB "FAT12   "   ; disk fs type
    RESB 18         ; empty 18 bytes

; prog-body
entry:
    MOV SI, 0
prt_msg:
    MOV AH, 0x0e    ; display one character start, INT 0x10
    MOV AL, [msg + SI]
    MOV BH, 0
    MOV BL, 9       ; color
    INT 0x10          ; display one character end
    
;    MOV SI, SI+1
    ADD SI, 1
    CMP AL, 0
    JE fin
    JMP prt_msg

fin:
    HLT
    JMP fin
        
; msg
msg:
    DB "Hello world!"

; last few bytes
    RESB 0x7dfe-$    ; set empty until 0x001fe
    DB 0x55, 0xaa   ; boot sector flag



    