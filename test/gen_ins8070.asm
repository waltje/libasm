;;; AUTO GENERATED FILE
;;; generated by: gen_ins8070 -u -C 8070 -o gen_ins8070.asm
      CPU 8070
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
      JSR  0x0022
      PLI  P2,#0x0023
      JMP  0x0026
      LD   SP,#0x0026
      LD   P2,#0x0027
      MPY  EA,T
      BND  $+0x0030
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
      BP   $+0x0067
      BP   103,P2
      BZ   $+0x006F
      BZ   111,P2
      ADD  A,E
      BRA  $+0x0077
      BRA  119,P2
      SUB  A,E
      BNZ  $+0x007F
      BNZ  127,P2
      LD   EA,$-0x007E,PC
      LD   EA,-126,SP
      LD   EA,-125,P2
      LD   EA,#0x0085
      LD   EA,0xFF86
      LD   EA,@-121,P2
      ST   EA,$-0x0076,PC
      ST   EA,-118,SP
      ST   EA,-117,P2
      ST   EA,0xFF8E
      ST   EA,@-113,P2
      ILD  A,$-0x006E,PC
      ILD  A,-110,SP
      ILD  A,-109,P2
      ILD  A,0xFF96
      ILD  A,@-105,P2
      DLD  A,$-0x0066,PC
      DLD  A,-102,SP
      DLD  A,-101,P2
      DLD  A,0xFF9E
      DLD  A,@-97,P2
      LD   T,$-0x005E,PC
      LD   T,-94,SP
      LD   T,-93,P2
      LD   T,#0x00A5
      LD   T,0xFFA6
      LD   T,@-89,P2
      ADD  EA,$-0x004E,PC
      ADD  EA,-78,SP
      ADD  EA,-77,P2
      ADD  EA,#0x00B5
      ADD  EA,0xFFB6
      ADD  EA,@-73,P2
      SUB  EA,$-0x0046,PC
      SUB  EA,-70,SP
      SUB  EA,-69,P2
      SUB  EA,#0x00BD
      SUB  EA,0xFFBE
      SUB  EA,@-65,P2
      LD   A,$-0x003E,PC
      LD   A,-62,SP
      LD   A,-61,P2
      LD   A,#0xC5
      LD   A,0xFFC6
      LD   A,@-57,P2
      ST   A,$-0x0036,PC
      ST   A,-54,SP
      ST   A,-53,P2
      ST   A,0xFFCE
      ST   A,@-49,P2
      AND  A,$-0x002E,PC
      AND  A,-46,SP
      AND  A,-45,P2
      AND  A,#0xD5
      AND  A,0xFFD6
      AND  A,@-41,P2
      OR   A,$-0x0026,PC
      OR   A,-38,SP
      OR   A,-37,P2
      OR   A,#0xDD
      OR   A,0xFFDE
      OR   A,@-33,P2
      XOR  A,$-0x001E,PC
      XOR  A,-30,SP
      XOR  A,-29,P2
      XOR  A,#0xE5
      XOR  A,0xFFE6
      XOR  A,@-25,P2
      ADD  A,$-14,PC
      ADD  A,-14,SP
      ADD  A,-13,P2
      ADD  A,#0xF5
      ADD  A,0xFFF6
      ADD  A,@-9,P2
      SUB  A,$-6,PC
      SUB  A,-6,SP
      SUB  A,-5,P2
      SUB  A,#0xFD
      SUB  A,0xFFFE
      SUB  A,@-1,P2
