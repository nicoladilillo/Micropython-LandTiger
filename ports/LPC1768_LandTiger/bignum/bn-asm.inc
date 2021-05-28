; Define and set macro variables BITS, WORDS and BYTES before including this file
;
; Functions place their arguments in R0, R1, R2, R3 and then after four they will be passed on the stack.
; The first four registers r0-r3 (a1-a4) are used to pass argument values into a subroutine and to return a result value from a function.
; They may also be used to hold intermediate values within a routine (but, in general, only between subroutine calls).
; A subroutine must preserve the contents of registers r4-r8, r10, r11 and the SP.
; This can be done on the stack with push and pop operations. See the ARM Procedure Call Standard for additional details on argument passing.
; Use   BX   LR  to return to C using link register (Branch indirect using LR - a return)

    EXPORT BnZer$BITS
    EXPORT BnInc$BITS
    EXPORT BnShr$BITS
    EXPORT BnShl$BITS
    EXPORT BnCpy$BITS
    EXPORT BnOrr$BITS
    EXPORT BnAdd$BITS
    EXPORT BnSub$BITS
    EXPORT BnCmp$BITS
    EXPORT BnIse$BITS
    EXPORT BnIne$BITS
    
    GBLA COUNT
    
BnZer$BITS
        MOV   R3, #0          ;Clear our 'zero' register
        
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        STR  R3, [R0], #4     ;R0 contains the address of the data; R0 will be post incremented by 4 bytes
    
        WEND
    
        BX   LR

BnInc$BITS
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R3, [R0]       ;R0 contains the address of the data
        ADDS R3,  R3,  #1   ;Add 1 without carry then update the carry flag
        STR  R3, [R0], #4   ;R0 contains the address of the data; R0 will be post incremented by 4 bytes
        BXCC LR             ;Return if the carry is clear
    
        WEND
    
        BX   LR

BnCpy$BITS
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R2, [R1], #4   ;R1 contains the address of the donor; R1 will be post incremented by 4 bytes
        STR  R2, [R0], #4   ;R0 contains the address of the recipient; R0 will be post incremented by 4 bytes
    
        WEND
    
        BX   LR
        
BnOrr$BITS
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R2, [R0]       ;R0 contains the address of the accumulator
        LDR  R3, [R1], #4   ;R1 contains the address of the value; R1 will be post incremented by 4 bytes
        ORR  R2,  R2,  R3   ;Or
        STR  R2, [R0], #4   ;R0 contains the address of the recipient; R0 will be post incremented by 4 bytes
    
        WEND
    
        BX   LR
        
BnAdd$BITS
        CMN  R0, #0         ;Acts like the addition of zero which will clear the carry flag
        
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R2, [R0]       ;R0 contains the address of the accumulator
        LDR  R3, [R1], #4   ;R1 contains the address of the additive; R1 will be post incremented by 4 bytes
        ADCS R2,  R2,  R3   ;Add with carry then update the carry flag
        STR  R2, [R0], #4   ;R0 contains the address of the data; R0 will be post incremented by 4 bytes
    
        WEND
    
        BX   LR
        
BnSub$BITS
        CMP  R0, #0         ;Acts like the subtraction of zero which will set the carry flag
        
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R2, [R0]       ;R0 contains the address of the accumulator
        LDR  R3, [R1], #4   ;R1 contains the address of the subtractor; R1 will be post incremented by 4 bytes
        SBCS R2,  R2,  R3   ;Subtract with carry then update the carry flag
        STR  R2, [R0], #4   ;R0 contains the address of the data; R0 will be post incremented by 4 bytes
    
        WEND
    
        BX   LR

BnShr$BITS                  ;IN R0 pData; IN R1 'bit to shift in'; OUT R0 'bit shifted out'
        ADD  R0, R0, #BYTES ;Go just beyond the big end of the data (1024 / 8)
        RRXS R1, R1         ;Put the lsb of 'bit to shift in' into the carry flag
        
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR  R3, [R0, #-4]! ;R0 contains the address of the data; it is pre-decremented by 4
        RRXS R3,  R3        ;Rotate right putting the carry into bit 31 then update the carry flag
        STR  R3, [R0]       ;R0 contains the address of the data
    
        WEND
    
        MOVCC R0, #0       ;Return carry set or carry clear
        MOVCS R0, #1
        BX   LR
        
BnShl$BITS                 ;IN R0 pData; IN R1 'bit to shift in'; OUT R0 'bit shifted out'
        AND   R1, R1, #1   ;Mask out all but the lsb in 'bit to shift in' in case a bool true is represented other than by a 1
        
COUNT   SETA  0
        WHILE COUNT < WORDS
COUNT   SETA  COUNT + 1

        LDR   R3, [R0]     ;R0 contains the address of the data
        LSLS  R3,  R3, #1  ;Shift R3 left by one then update the carry flag with the 31st bit
        ORR   R3,  R1      ;Add the 'bit to shift in' but don't touch the carry flag
        MOVCC R1, #0       ;Set 'bit to shift in' for the next loop from the carry set or carry clear
        MOVCS R1, #1
        STR   R3, [R0], #4 ;R0 contains the address of the data; R0 will be post incremented by 4 bytes
    
        WEND
        MOV   R0, R1       ;Return the 'bit shifted out'
        BX    LR
        
BnCmp$BITS
        ADD  R0, R0, #BYTES    ;Go just beyond the big end of the data (1024 / 8)
        ADD  R1, R1, #BYTES    ;Go just beyond the big end of the data (1024 / 8)
        
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR   R2, [R0, #-4]! ;R0 contains the address of the lhs; R0 will be pre decremented by 4 bytes
        LDR   R3, [R1, #-4]! ;R1 contains the address of the rhs; R1 will be pre decremented by 4 bytes
        CMP   R2,  R3        ;Set flags as result of subtracting R3 from R2. R2 > R3 ==> HI; R2 < R3 ==> LO
        BHI.W %F99           ;Return +1 if R2 is higher than R3
        BLO.W %F98           ;Return -1 if R2 is higher than R3
    
        WEND
    
        MOV  R0, #0          ;Return  0
        BX   LR              ;
99      MOV  R0, #+1         ;Return +1
        BX   LR
98      MOV  R0, #-1         ;Return -1
        BX   LR

BnIse$BITS
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR   R3, [R0], #4   ;R0 contains the address of the lhs; R0 will be post incremented by 4 bytes
        CMP   R3,  #0        ;Set flags as result of subtracting R3 from R2. R2 > R3 ==> HI; R2 < R3 ==> LO
        BNE.W %F99           ;Return not empty - F == search forward 0 too 99 are local numeric labels
    
        WEND

        MOV  R0, #1          ;Return true
        BX   LR
99      MOV  R0, #0          ;Return false
        BX   LR

        
BnIne$BITS
COUNT   SETA    0
        WHILE   COUNT < WORDS
COUNT   SETA    COUNT + 1

        LDR   R3, [R0], #4   ;R0 contains the address of the lhs; R0 will be post incremented by 4 bytes
        CMP   R3,  #0        ;Set flags as result of subtracting R3 from R2. R2 > R3 ==> HI; R2 < R3 ==> LO
        BNE.W %F99           ;Return not empty
    
        WEND

        MOV  R0, #0          ;Return false
        BX   LR
99      MOV  R0, #1          ;Return true
        BX   LR
