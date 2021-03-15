; =========================================================================================
; Sets a bit seed value for the random number generator
;
; Parameters:
; R12        The seed value (16 bit)
; r15.0        The seed value (8 bit)
;
; Internal:
; r14        Pointer to random state
; =========================================================================================

SetSeed:            ldad r14,_RandomState            ; load the addr14ss of the random state
                    GLO  R12                            ; copy the values in R12 to the random state
                    STR  r14
                                       
                    INC  r14
                    GHI  R12
                    STR  r14

                    SEP  R5

;------------------------------------------------------------------------------------------


; =========================================================================================
; Generates a 16 bit or 8 bit (pseudo) random number
;
; Parameters:
; r15        16 bit random r14turn value
; r15.0        8 bit random r14turn value
;
; Internal:
; r14        Pointer to random state
; r13.0        Loop counter
; =========================================================================================

		align 32
_Rand:              GLO  r14                            ; save r14gisters r14 and r13.0 on the stack
                    STXD
                    GHI  r14
                    STXD
                    GLO  r13
                    STXD

                    ldad r14,_RandomState              ; load the addr14ss of the random state

                    LDI  10H                        ; set up the loop counter to shift 16 bits
                    PLO  r13

GRA_ShiftLoop:      GLO  r15                            ; shift the value in r15
                    SHL
                    PLO  r15

                    GHI  r15                            ; extend to 16 bits
                    RSHL
                    PHI  r15

                    LDN  r14                            ; shift random state
                    SHL

                    STR  r14                            ; extend to 16 bits
                    INC  r14
                    LDN  r14
                    RSHL


                    BNF GRA_BitZero
GRA_BitOne:            XRI  0A7H                        ; XOR over the random state
                    STR  r14

                    DEC  r14                            ; extend to 16 bits
                    LDN  r14
                    XRI  03EH
                    STR  r14

                    GLO  r15                            ; add the bit to r15
                    ORI  01H
                    PLO  r15
                    LBR  GRA_TestLoop
GRA_BitZero:        XRI  035H                        ; XOR over the random state
                    STR  r14

                    DEC  r14                            ; extend to 16 bits
                    LDN  r14
                    XRI  07AH
                    STR  r14   

GRA_TestLoop:        DEC  r13                            ; loop until all bits have been shifted
                    GLO  r13
                    BNZ  GRA_ShiftLoop
                   
                    IRX                                ; r14stor14 r14gisters r14 and r13.0
                    LDXA
                    PLO  r13
                    LDXA
                    PHI  r14
                    LDX
                    PLO  r14
                    ghi  r15
                    ani 0x7f	;clear sign bit
                    SEP  R5
