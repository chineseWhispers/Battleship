    AREA setup, CODE, READONLY
;--------------------OVERVIEW------------------------
; This file contains ARM Assembly language functions to
; initialize select registers/memory-locations in the
; LPC1768 to predetermined values.  This startup sequence
; will be used to test the integrity of your program, and
; IT SHOULD NOT BE ALTERED BY ANY MEANS.  Any student
; attempting to alter this file will be subject to academic
; dishonesty and any disciplinary actions pertaining
; thereto.
;------------------------------------------------------
; Export function location so that C compiler can find it and link
    EXPORT setup_sequence
setup_sequence
    ;PUSH LINK REGISTER TO STACK
    PUSH    {LR}
    
    ;INIT PINSEL0
    LDR     R0, =0x4002C000     ;LOAD ADDRESS OF PINSEL0 REGISTER
    LDR     R1, =0x40000000     ;LOAD BITMASK FOR PINSEL0 REGISTER
    LDR     R2, [R0]            ;\ 
    ORR     R2, R1              ;- APPLY BITMASK
    STR     R2, [R0]            ;/ 
    
    ;INIT PINSEL1
    LDR     R0, =0x4002C004     ;LOAD ADDRESS OF PINSEL1 REGISTER
    LDR     R1, =0x00054015     ;LOAD BITMASK FOR PINSEL1 REGISTER
    LDR     R2, [R0]            ;\ 
    ORR     R2, R1              ;- APPLY BITMASK
    STR     R2, [R0]            ;/ 
    
    ;INIT PINSEL3
    LDR     R0, =0x4002C00C     ;LOAD ADDRESS OF PINSEL3 REGISTER
    LDR     R1, =0x00004510     ;LOAD BITMASK FOR PINSEL4 REGISTER
    LDR     R2, [R0]            ;\ 
    ORR     R2, R1              ;- APPLY BITMASK
    STR     R2, [R0]            ;/ 
    
    ;RETURN TO MAIN
    POP     {LR}
    BX      LR 
    END