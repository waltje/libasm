;;; AUTO GENERATED FILE
;;; generated by: gen_z8 -u -C Z88C00 -o gen_z88.asm
      CPU     Z88C00
      ORG     0100H
      DEC     >01H
      DEC     10H
      DEC     @02H
      ADD     R0,R3
      ADD     R0,@R4
      ADD     >05H,>00H
      ADD     10H,>00H
      ADD     >06H,@00H
      ADD     10H,@00H
      ADD     >00H,#7
      BOR     R0,>08H,#0
      BOR     R0,10H,#0
      BOR     >00H,#0,R0
      BOR     10H,#0,R0
      LD      R0,>09H
      LD      R0,10H
      LD      >0AH,R0
      LD      10H,R0
      DJNZ    R0,$+13
      DJNZ    R0,$-126
      DJNZ    R0,$
      JR      F,$+14
      JR      F,$-126
      JR      F,$
      LD      R0,#13
      JP      F,000EH
      INC     R0
      NEXT
      RLC     11H
      RLC     >00H
      RLC     @12H
      ADC     R1,R3
      ADC     R1,@R4
      ADC     15H,>00H
      ADC     >00H,>01H
      ADC     16H,@00H
      ADC     >00H,@01H
      ADC     >00H,#23
      JR      LT,$+30
      JR      LT,$-126
      JR      LT,$
      JP      LT,001EH
      ENTER
      INC     21H
      INC     >00H
      INC     @22H
      SUB     R2,R3
      SUB     R2,@R4
      SUB     25H,>00H
      SUB     >00H,>01H
      SUB     26H,@00H
      SUB     >00H,@01H
      SUB     >00H,#27H
      BXOR    R0,28H,#0
      BXOR    >00H,#0,R0
      BXOR    10H,#0,R0
      BXOR    R0,>00H,#1
      JR      LE,$+46
      JR      LE,$-126
      JR      LE,$
      JP      LE,002EH
      EXIT
      SRP0    #30H
      SRP1    #38H
      SRP     #40H
      SBC     R3,R3
      SBC     R3,@R4
      SBC     35H,>00H
      SBC     >00H,>01H
      SBC     36H,@00H
      SBC     >00H,@01H
      SBC     >00H,#37H
      BTJRF   $+59,R0,#0
      BTJRF   $-125,R0,#0
      BTJRF   $,R0,#0
      BTJRT   $+3,R0,#0
      BTJRT   $-125,R0,#0
      BTJRT   $,R0,#0
      JR      ULE,$+62
      JR      ULE,$-126
      JR      ULE,$
      JP      ULE,003EH
      WFI
      DA      41H
      DA      >00H
      DA      @42H
      OR      R4,R3
      OR      R4,@R4
      OR      45H,>00H
      OR      >00H,>01H
      OR      46H,@00H
      OR      >00H,@01H
      OR      >00H,#47H
      LDB     R0,48H,#0
      LDB     >00H,#0,R0
      LDB     10H,#0,R0
      LDB     R0,>00H,#1
      JR      OV,$+78
      JR      OV,$-126
      JR      OV,$
      JP      OV,004EH
      SB0
      POP     51H
      POP     >00H
      POP     @52H
      AND     R5,R3
      AND     R5,@R4
      AND     55H,>00H
      AND     >00H,>01H
      AND     56H,@00H
      AND     >00H,@01H
      AND     >00H,#57H
      JR      MI,$+94
      JR      MI,$-126
      JR      MI,$
      JP      MI,005EH
      SB1
      COM     61H
      COM     >00H
      COM     @62H
      TCM     R6,R3
      TCM     R6,@R4
      TCM     65H,>00H
      TCM     >00H,>01H
      TCM     66H,@00H
      TCM     >00H,@01H
      TCM     >00H,#67H
      BAND    R0,68H,#0
      BAND    >00H,#0,R0
      BAND    10H,#0,R0
      BAND    R0,>00H,#1
      JR      Z,$+110
      JR      Z,$-126
      JR      Z,$
      JP      Z,006EH
      PUSH    71H
      PUSH    >00H
      PUSH    @72H
      TM      R7,R3
      TM      R7,@R4
      TM      75H,>00H
      TM      >00H,>01H
      TM      76H,@00H
      TM      >00H,@01H
      TM      >00H,#77H
      BITR    R7,#4
      BITS    R7,#4
      JR      C,$+126
      JR      C,$-126
      JR      C,$
      JP      C,007EH
      DECW    @82H
      PUSHUD  @00H,83H
      PUSHUD  @01H,>00H
      PUSHUI  @00H,84H
      PUSHUI  @01H,>00H
      MULT    86H,>00H
      MULT    >00H,>01H
      MULT    86H,@00H
      MULT    >00H,@01H
      MULT    88H,#0
      MULT    >00H,#1
      LD      R0,88H(R0)
      JR      $-114
      JR      $
      JR      $+1
      JP      008EH
      DI
      RL      91H
      RL      >00H
      RL      @92H
      POPUD   93H,@00H
      POPUD   >00H,@01H
      POPUI   94H,@00H
      POPUI   >00H,@01H
      DIV     96H,>00H
      DIV     >00H,>01H
      DIV     96H,@00H
      DIV     >00H,@01H
      DIV     98H,#0
      DIV     >00H,#1
      LD      98H(R0),R0
      JR      GE,$-98
      JR      GE,$
      JR      GE,$+1
      JP      GE,009EH
      EI
      INCW    @0A2H
      CP      R10,R3
      CP      R10,@R4
      CP      0A5H,>00H
      CP      >00H,>01H
      CP      0A6H,@00H
      CP      >00H,@01H
      CP      >00H,#0A7H
      LDC     R10,0A900H(RR8)
      LDE     R10,0AA00H(RR8)
      LDC     R11,0B100H
      LDE     R11,0B200H
      JR      GT,$-82
      JR      GT,$
      JR      GT,$+1
      JP      GT,00AEH
      RET
      CLR     0B1H
      CLR     >00H
      CLR     @0B2H
      XOR     R11,R3
      XOR     R11,@R4
      XOR     0B5H,>00H
      XOR     >00H,>01H
      XOR     0B6H,@00H
      XOR     >00H,@01H
      XOR     >00H,#0B7H
      LDC     0B900H(RR8),R11
      LDE     0BA00H(RR8),R11
      LDC     0C100H,R12
      LDE     0C200H,R12
      JR      UGT,$-66
      JR      UGT,$
      JR      UGT,$+1
      JP      UGT,00BEH
      IRET
      RRC     0C1H
      RRC     >00H
      RRC     @0C2H
      CPIJE   R0,@R0,$-58
      CPIJE   R0,@R0,$
      CPIJE   R0,@R0,$+1
      LDC     R12,@RR4
      LDE     R12,@RR4
      LDW     0C8H,#00C9H
      LDW     >00H,#0101H
      LD      R12,@R8
      JR      NOV,$-50
      JR      NOV,$
      JR      NOV,$+1
      JP      NOV,00CEH
      RCF
      SRA     0D1H
      SRA     >00H
      SRA     @0D2H
      CPIJNE  R0,@R0,$-42
      CPIJNE  R0,@R0,$
      CPIJNE  R0,@R0,$+1
      LDC     @RR4,R13
      LDE     @RR4,R13
      CALL    #0D5H
      LD      @00H,#0D7H
      LD      @R13,R8
      JR      PL,$-34
      JR      PL,$
      JR      PL,$+1
      JP      PL,00DEH
      SCF
      RR      0E1H
      RR      >00H
      RR      @0E2H
      LDED    R14,@RR2
      LDCD    R14,@RR4
      LDCI    R14,@RR4
      LDEI    R14,@RR4
      LD      0E5H,>00H
      LD      >00H,>01H
      LD      0E6H,@00H
      LD      >00H,@01H
      LD      >00H,#0E7H
      LDC     R0,-24(RR0)
      LDE     R0,0(RR0)
      LDE     R0,+1(RR0)
      LDE     R0,-128(RR0)
      LDC     R0,0(RR2)
      LDC     R0,+1(RR2)
      JR      NZ,$-18
      JR      NZ,$
      JR      NZ,$+1
      JP      NZ,00EEH
      CCF
      SWAP    0F1H
      SWAP    >00H
      SWAP    @0F2H
      LDEPD   @RR2,R15
      LDCPD   @RR4,R15
      LDCPI   @RR4,R15
      LDEPI   @RR4,R15
      CALL    @0F6H
      LD      @0F6H,>00H
      CALL    00F7H
      LDC     -8(RR0),R0
      LDE     0(RR0),R0
      LDE     +1(RR0),R0
      LDE     -128(RR0),R0
      LDC     0(RR2),R0
      LDC     +1(RR2),R0
      JR      NC,$-2
      JR      NC,$
      JR      NC,$+1
      JP      NC,00FEH
      NOP
