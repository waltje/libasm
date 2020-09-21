;;; AUTO GENERATED FILE
;;; generated by: gen_z80 -u -C 8085 -o gen_i8085_z80syn.asm
      CPU 8085
      Z80SYNTAX EXCLUSIVE
      NOP
      LD   BC,2
      LD   (BC),A
      INC  BC
      INC  B
      DEC  B
      LD   B,7
      RLCA
      ADD  HL,BC
      LD   A,(BC)
      DEC  BC
      INC  C
      DEC  C
      LD   C,15
      RRCA
      LD   DE,18
      LD   (DE),A
      INC  DE
      INC  D
      DEC  D
      LD   D,23
      RLA
      ADD  HL,DE
      LD   A,(DE)
      DEC  DE
      INC  E
      DEC  E
      LD   E,31
      RRA
      LD   A,IM
      LD   HL,0022H
      LD   (0023H),HL
      INC  HL
      INC  H
      DEC  H
      LD   H,27H
      DAA
      ADD  HL,HL
      LD   HL,(002BH)
      DEC  HL
      INC  L
      DEC  L
      LD   L,2FH
      CPL
      LD   IM,A
      LD   SP,0032H
      LD   (0033H),A
      INC  SP
      INC  (HL)
      DEC  (HL)
      LD   (HL),37H
      SCF
      ADD  HL,SP
      LD   A,(003BH)
      DEC  SP
      INC  A
      DEC  A
      LD   A,3FH
      CCF
      LD   B,B
      LD   B,C
      LD   B,D
      LD   B,E
      LD   B,H
      LD   B,L
      LD   B,(HL)
      LD   B,A
      LD   C,B
      LD   C,C
      LD   C,D
      LD   C,E
      LD   C,H
      LD   C,L
      LD   C,(HL)
      LD   C,A
      LD   D,B
      LD   D,C
      LD   D,D
      LD   D,E
      LD   D,H
      LD   D,L
      LD   D,(HL)
      LD   D,A
      LD   E,B
      LD   E,C
      LD   E,D
      LD   E,E
      LD   E,H
      LD   E,L
      LD   E,(HL)
      LD   E,A
      LD   H,B
      LD   H,C
      LD   H,D
      LD   H,E
      LD   H,H
      LD   H,L
      LD   H,(HL)
      LD   H,A
      LD   L,B
      LD   L,C
      LD   L,D
      LD   L,E
      LD   L,H
      LD   L,L
      LD   L,(HL)
      LD   L,A
      LD   (HL),B
      LD   (HL),C
      LD   (HL),D
      LD   (HL),E
      LD   (HL),H
      LD   (HL),L
      HALT
      LD   (HL),A
      LD   A,B
      LD   A,C
      LD   A,D
      LD   A,E
      LD   A,H
      LD   A,L
      LD   A,(HL)
      LD   A,A
      ADD  A,B
      ADD  A,C
      ADD  A,D
      ADD  A,E
      ADD  A,H
      ADD  A,L
      ADD  A,(HL)
      ADD  A,A
      ADC  A,B
      ADC  A,C
      ADC  A,D
      ADC  A,E
      ADC  A,H
      ADC  A,L
      ADC  A,(HL)
      ADC  A,A
      SUB  A,B
      SUB  A,C
      SUB  A,D
      SUB  A,E
      SUB  A,H
      SUB  A,L
      SUB  A,(HL)
      SUB  A,A
      SBC  A,B
      SBC  A,C
      SBC  A,D
      SBC  A,E
      SBC  A,H
      SBC  A,L
      SBC  A,(HL)
      SBC  A,A
      AND  A,B
      AND  A,C
      AND  A,D
      AND  A,E
      AND  A,H
      AND  A,L
      AND  A,(HL)
      AND  A,A
      XOR  A,B
      XOR  A,C
      XOR  A,D
      XOR  A,E
      XOR  A,H
      XOR  A,L
      XOR  A,(HL)
      XOR  A,A
      OR   A,B
      OR   A,C
      OR   A,D
      OR   A,E
      OR   A,H
      OR   A,L
      OR   A,(HL)
      OR   A,A
      CP   A,B
      CP   A,C
      CP   A,D
      CP   A,E
      CP   A,H
      CP   A,L
      CP   A,(HL)
      CP   A,A
      RET  NZ
      POP  BC
      JP   NZ,00C3H
      JP   00C4H
      CALL NZ,00C5H
      PUSH BC
      ADD  A,0C7H
      RST  0
      RET  Z
      RET
      JP   Z,00CBH
      CALL Z,00CDH
      CALL 00CEH
      ADC  A,0CFH
      RET  NC
      POP  DE
      JP   NC,00D3H
      OUT  (0D4H),A
      CALL NC,00D5H
      PUSH DE
      SUB  A,0D7H
      RET  C
      JP   C,00DBH
      IN   A,(0DCH)
      CALL C,00DDH
      SBC  A,0DFH
      RET  PO
      POP  HL
      JP   PO,00E3H
      EX   (SP),HL
      CALL PO,00E5H
      PUSH HL
      AND  A,0E7H
      RET  PE
      JP   (HL)
      JP   PE,00EBH
      EX   DE,HL
      CALL PE,00EDH
      XOR  A,0EFH
      RET  P
      POP  AF
      JP   P,00F3H
      DI
      CALL P,00F5H
      PUSH AF
      OR   A,0F7H
      RET  M
      LD   SP,HL
      JP   M,00FBH
      EI
      CALL M,00FDH
      CP   A,0FFH
