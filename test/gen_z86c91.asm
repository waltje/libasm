;;; AUTO GENERATED FILE
;;; generated by: gen_z8 -u -C Z86C91 -o gen_z86c91.asm
      CPU Z86C91
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
      ADD     @00H,#8
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
      ADC     @00H,#24
      JR      LT,$+30
      JR      LT,$-126
      JR      LT,$
      JP      LT,001EH
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
      SUB     @00H,#28H
      JR      LE,$+46
      JR      LE,$-126
      JR      LE,$
      JP      LE,002EH
      SRP     #32H
      SBC     R3,R3
      SBC     R3,@R4
      SBC     35H,>00H
      SBC     >00H,>01H
      SBC     36H,@00H
      SBC     >00H,@01H
      SBC     >00H,#37H
      SBC     @00H,#38H
      JR      ULE,$+62
      JR      ULE,$-126
      JR      ULE,$
      JP      ULE,003EH
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
      OR      @00H,#48H
      JR      OV,$+78
      JR      OV,$-126
      JR      OV,$
      JP      OV,004EH
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
      AND     @00H,#58H
      JR      MI,$+94
      JR      MI,$-126
      JR      MI,$
      JP      MI,005EH
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
      TCM     @00H,#68H
      JR      Z,$+110
      JR      Z,$-126
      JR      Z,$
      JP      Z,006EH
      STOP
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
      TM      @00H,#78H
      JR      C,$+126
      JR      C,$-126
      JR      C,$
      JP      C,007EH
      HALT
      DECW    @82H
      LDE     R8,@RR4
      LDEI    @R8,@RR4
      JR      $-114
      JR      $
      JR      $+1
      JP      008EH
      DI
      RL      91H
      RL      >00H
      RL      @92H
      LDE     @RR4,R9
      LDEI    @RR4,@R9
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
      CP      @00H,#0A8H
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
      XOR     @00H,#0B8H
      JR      UGT,$-66
      JR      UGT,$
      JR      UGT,$+1
      JP      UGT,00BEH
      IRET
      RRC     0C1H
      RRC     >00H
      RRC     @0C2H
      LDC     R12,@RR4
      LDCI    @R12,@RR4
      LD      R0,0C8H(R0)
      JR      NOV,$-50
      JR      NOV,$
      JR      NOV,$+1
      JP      NOV,00CEH
      RCF
      SRA     0D1H
      SRA     >00H
      SRA     @0D2H
      LDC     @RR4,R13
      LDCI    @RR4,@R13
      CALL    @0D6H
      CALL    00D7H
      LD      0D8H(R0),R0
      JR      PL,$-34
      JR      PL,$
      JR      PL,$+1
      JP      PL,00DEH
      SCF
      RR      0E1H
      RR      >00H
      RR      @0E2H
      LD      R14,@R4
      LD      0E5H,>00H
      LD      >00H,>01H
      LD      0E6H,@00H
      LD      >00H,@01H
      LD      >00H,#0E7H
      LD      @00H,#0E8H
      JR      NZ,$-18
      JR      NZ,$
      JR      NZ,$+1
      JP      NZ,00EEH
      CCF
      SWAP    0F1H
      SWAP    >00H
      SWAP    @0F2H
      LD      @R15,R4
      LD      @0F6H,>00H
      JR      NC,$-2
      JR      NC,$
      JR      NC,$+1
      JP      NC,00FEH
      NOP
