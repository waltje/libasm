;;; AUTO GENERATED FILE
;;; generated by: gen_ins8070 -u -C 8070 -o gen_ins8070.asm -l gen_ins8070.lst
      CPU  8070
      ORG  0x0100
      NOP
      XCH  A,E
      LD   A,S
      LD   S,A
      PUSH EA
      LD   T,EA
      PUSH A
      LD   EA,T
      SR   EA
      DIV  EA,T
      SL   A
      SL   EA
      CALL 0
      JSR  0x2222
      PLI  P2,#0x2423
      JMP  0x2626
      LD   SP,#0x2726
      LD   P2,#0x2827
      MPY  EA,T
      BND  $+48
      BND  $-126
      BND  $
      SSM  P2
      LD   EA,PC
      LD   EA,SP
      LD   EA,P2
      POP  A
      AND  S,#0x3A
      POP  EA
      OR   S,#0x3C
      SR   A
      SRL  A
      RR   A
      RRL  A
      LD   A,E
      LD   PC,EA
      LD   SP,EA
      LD   P2,EA
      LD   E,A
      XCH  EA,PC
      XCH  EA,SP
      XCH  EA,P2
      AND  A,E
      PUSH PC
      PUSH P2
      OR   A,E
      RET
      POP  P2
      XOR  A,E
      BP   $+103
      BP   $-126
      BP   $
      BP   103,P2
      BP   -128,P2
      BZ   $+111
      BZ   $-126
      BZ   $
      BZ   111,P2
      BZ   -128,P2
      ADD  A,E
      BRA  $+119
      BRA  $-126
      BRA  $
      BRA  119,P2
      BRA  -128,P2
      SUB  A,E
      BNZ  $+127
      BNZ  $-126
      BNZ  $
      BNZ  127,P2
      BNZ  -128,P2
      LD   EA,$-126,PC
      LD   EA,$,PC
      LD   EA,$+1,PC
      LD   EA,-126,SP
      LD   EA,0,SP
      LD   EA,-125,P2
      LD   EA,0,P2
      LD   EA,#0x8685
      LD   EA,0xFF86
      LD   EA,@-121,P2
      LD   EA,@0,P2
      ST   EA,$-118,PC
      ST   EA,$,PC
      ST   EA,$+1,PC
      ST   EA,-118,SP
      ST   EA,0,SP
      ST   EA,-117,P2
      ST   EA,0,P2
      ST   EA,0xFF8E
      ST   EA,@-113,P2
      ST   EA,@0,P2
      ILD  A,$-110,PC
      ILD  A,$,PC
      ILD  A,$+1,PC
      ILD  A,-110,SP
      ILD  A,0,SP
      ILD  A,-109,P2
      ILD  A,0,P2
      ILD  A,0xFF96
      ILD  A,@-105,P2
      ILD  A,@0,P2
      DLD  A,$-102,PC
      DLD  A,$,PC
      DLD  A,$+1,PC
      DLD  A,-102,SP
      DLD  A,0,SP
      DLD  A,-101,P2
      DLD  A,0,P2
      DLD  A,0xFF9E
      DLD  A,@-97,P2
      DLD  A,@0,P2
      LD   T,$-94,PC
      LD   T,$,PC
      LD   T,$+1,PC
      LD   T,-94,SP
      LD   T,0,SP
      LD   T,-93,P2
      LD   T,0,P2
      LD   T,#0xA6A5
      LD   T,0xFFA6
      LD   T,@-89,P2
      LD   T,@0,P2
      ADD  EA,$-78,PC
      ADD  EA,$,PC
      ADD  EA,$+1,PC
      ADD  EA,-78,SP
      ADD  EA,0,SP
      ADD  EA,-77,P2
      ADD  EA,0,P2
      ADD  EA,#0xB6B5
      ADD  EA,0xFFB6
      ADD  EA,@-73,P2
      ADD  EA,@0,P2
      SUB  EA,$-70,PC
      SUB  EA,$,PC
      SUB  EA,$+1,PC
      SUB  EA,-70,SP
      SUB  EA,0,SP
      SUB  EA,-69,P2
      SUB  EA,0,P2
      SUB  EA,#0xBEBD
      SUB  EA,0xFFBE
      SUB  EA,@-65,P2
      SUB  EA,@0,P2
      LD   A,$-62,PC
      LD   A,$,PC
      LD   A,$+1,PC
      LD   A,-62,SP
      LD   A,0,SP
      LD   A,-61,P2
      LD   A,0,P2
      LD   A,#0xC5
      LD   A,0xFFC6
      LD   A,@-57,P2
      LD   A,@0,P2
      ST   A,$-54,PC
      ST   A,$,PC
      ST   A,$+1,PC
      ST   A,-54,SP
      ST   A,0,SP
      ST   A,-53,P2
      ST   A,0,P2
      ST   A,0xFFCE
      ST   A,@-49,P2
      ST   A,@0,P2
      AND  A,$-46,PC
      AND  A,$,PC
      AND  A,$+1,PC
      AND  A,-46,SP
      AND  A,0,SP
      AND  A,-45,P2
      AND  A,0,P2
      AND  A,#0xD5
      AND  A,0xFFD6
      AND  A,@-41,P2
      AND  A,@0,P2
      OR   A,$-38,PC
      OR   A,$,PC
      OR   A,$+1,PC
      OR   A,-38,SP
      OR   A,0,SP
      OR   A,-37,P2
      OR   A,0,P2
      OR   A,#0xDD
      OR   A,0xFFDE
      OR   A,@-33,P2
      OR   A,@0,P2
      XOR  A,$-30,PC
      XOR  A,$,PC
      XOR  A,$+1,PC
      XOR  A,-30,SP
      XOR  A,0,SP
      XOR  A,-29,P2
      XOR  A,0,P2
      XOR  A,#0xE5
      XOR  A,0xFFE6
      XOR  A,@-25,P2
      XOR  A,@0,P2
      ADD  A,$-14,PC
      ADD  A,$,PC
      ADD  A,$+1,PC
      ADD  A,-14,SP
      ADD  A,0,SP
      ADD  A,-13,P2
      ADD  A,0,P2
      ADD  A,#0xF5
      ADD  A,0xFFF6
      ADD  A,@-9,P2
      ADD  A,@0,P2
      SUB  A,$-6,PC
      SUB  A,$,PC
      SUB  A,$+1,PC
      SUB  A,-6,SP
      SUB  A,0,SP
      SUB  A,-5,P2
      SUB  A,0,P2
      SUB  A,#0xFD
      SUB  A,0xFFFE
      SUB  A,@-1,P2
      SUB  A,@0,P2
