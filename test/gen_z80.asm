;;; AUTO GENERATED FILE
;;; generated by: gen_z80 -u -C Z80 -o gen_z80.asm
      CPU Z80
      NOP
      LD   BC,2
      LD   (BC),A
      INC  BC
      INC  B
      DEC  B
      LD   B,7
      RLCA
      EX   AF,AF'
      ADD  HL,BC
      LD   A,(BC)
      DEC  BC
      INC  C
      DEC  C
      LD   C,15
      RRCA
      DJNZ $+19
      DJNZ $-7EH
      DJNZ $
      LD   DE,18
      LD   (DE),A
      INC  DE
      INC  D
      DEC  D
      LD   D,23
      RLA
      JR   $+27
      JR   $-7EH
      JR   $
      ADD  HL,DE
      LD   A,(DE)
      DEC  DE
      INC  E
      DEC  E
      LD   E,31
      RRA
      JR   NZ,$+23H
      JR   NZ,$-7EH
      JR   NZ,$
      LD   HL,0022H
      LD   (0023H),HL
      INC  HL
      INC  H
      DEC  H
      LD   H,27H
      DAA
      JR   Z,$+2BH
      JR   Z,$-7EH
      JR   Z,$
      ADD  HL,HL
      LD   HL,(002BH)
      DEC  HL
      INC  L
      DEC  L
      LD   L,2FH
      CPL
      JR   NC,$+33H
      JR   NC,$-7EH
      JR   NC,$
      LD   SP,0032H
      LD   (0033H),A
      INC  SP
      INC  (HL)
      DEC  (HL)
      LD   (HL),37H
      SCF
      JR   C,$+3BH
      JR   C,$-7EH
      JR   C,$
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
      RST  00H
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
      EXX
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
      IN   B,(C)
      OUT  (C),B
      SBC  HL,BC
      LD   (0044H),BC
      NEG
      RETN
      IM   0
      LD   I,A
      IN   C,(C)
      OUT  (C),C
      ADC  HL,BC
      LD   BC,(004CH)
      RETI
      LD   R,A
      IN   D,(C)
      OUT  (C),D
      SBC  HL,DE
      LD   (0054H),DE
      LD   A,I
      IN   E,(C)
      OUT  (C),E
      ADC  HL,DE
      LD   DE,(005CH)
      LD   A,R
      IN   H,(C)
      OUT  (C),H
      SBC  HL,HL
      RRD
      IN   L,(C)
      OUT  (C),L
      ADC  HL,HL
      RLD
      SBC  HL,SP
      LD   (0074H),SP
      IN   A,(C)
      OUT  (C),A
      ADC  HL,SP
      LD   SP,(007CH)
      LDI
      CPI
      INI
      OUTI
      LDD
      CPD
      IND
      OUTD
      LDIR
      CPIR
      INIR
      OTIR
      LDDR
      CPDR
      INDR
      OTDR
      ADD  IX,BC
      ADD  IX,DE
      LD   IX,0022H
      LD   (0023H),IX
      INC  IX
      ADD  IX,IX
      LD   IX,(002BH)
      DEC  IX
      INC  (IX+53)
      INC  (IX-128)
      DEC  (IX+54)
      DEC  (IX-128)
      LD   (IX+55),0
      LD   (IX-128),0
      ADD  IX,SP
      LD   B,(IX+71)
      LD   B,(IX-128)
      LD   C,(IX+79)
      LD   C,(IX-128)
      LD   D,(IX+87)
      LD   D,(IX-128)
      LD   E,(IX+95)
      LD   E,(IX-128)
      LD   H,(IX+103)
      LD   H,(IX-128)
      LD   L,(IX+111)
      LD   L,(IX-128)
      LD   (IX+113),B
      LD   (IX-128),B
      LD   (IX+114),C
      LD   (IX-128),C
      LD   (IX+115),D
      LD   (IX-128),D
      LD   (IX+116),E
      LD   (IX-128),E
      LD   (IX+117),H
      LD   (IX-128),H
      LD   (IX+118),L
      LD   (IX-128),L
      LD   (IX+120),A
      LD   (IX-128),A
      LD   A,(IX+127)
      LD   A,(IX-128)
      ADD  A,(IX-121)
      ADD  A,(IX+0)
      ADC  A,(IX-113)
      ADC  A,(IX+0)
      SUB  A,(IX-105)
      SUB  A,(IX+0)
      SBC  A,(IX-97)
      SBC  A,(IX+0)
      AND  A,(IX-89)
      AND  A,(IX+0)
      XOR  A,(IX-81)
      XOR  A,(IX+0)
      OR   A,(IX-73)
      OR   A,(IX+0)
      CP   A,(IX-65)
      CP   A,(IX+0)
      POP  IX
      EX   (SP),IX
      PUSH IX
      JP   (IX)
      LD   SP,IX
      ADD  IY,BC
      ADD  IY,DE
      LD   IY,0022H
      LD   (0023H),IY
      INC  IY
      ADD  IY,IY
      LD   IY,(002BH)
      DEC  IY
      INC  (IY+53)
      INC  (IY-128)
      DEC  (IY+54)
      DEC  (IY-128)
      LD   (IY+55),0
      LD   (IY-128),0
      ADD  IY,SP
      LD   B,(IY+71)
      LD   B,(IY-128)
      LD   C,(IY+79)
      LD   C,(IY-128)
      LD   D,(IY+87)
      LD   D,(IY-128)
      LD   E,(IY+95)
      LD   E,(IY-128)
      LD   H,(IY+103)
      LD   H,(IY-128)
      LD   L,(IY+111)
      LD   L,(IY-128)
      LD   (IY+113),B
      LD   (IY-128),B
      LD   (IY+114),C
      LD   (IY-128),C
      LD   (IY+115),D
      LD   (IY-128),D
      LD   (IY+116),E
      LD   (IY-128),E
      LD   (IY+117),H
      LD   (IY-128),H
      LD   (IY+118),L
      LD   (IY-128),L
      LD   (IY+120),A
      LD   (IY-128),A
      LD   A,(IY+127)
      LD   A,(IY-128)
      ADD  A,(IY-121)
      ADD  A,(IY+0)
      ADC  A,(IY-113)
      ADC  A,(IY+0)
      SUB  A,(IY-105)
      SUB  A,(IY+0)
      SBC  A,(IY-97)
      SBC  A,(IY+0)
      AND  A,(IY-89)
      AND  A,(IY+0)
      XOR  A,(IY-81)
      XOR  A,(IY+0)
      OR   A,(IY-73)
      OR   A,(IY+0)
      CP   A,(IY-65)
      CP   A,(IY+0)
      POP  IY
      EX   (SP),IY
      PUSH IY
      JP   (IY)
      LD   SP,IY
      RLC  B
      RLC  C
      RLC  D
      RLC  E
      RLC  H
      RLC  L
      RLC  (HL)
      RLC  A
      RRC  B
      RRC  C
      RRC  D
      RRC  E
      RRC  H
      RRC  L
      RRC  (HL)
      RRC  A
      RL   B
      RL   C
      RL   D
      RL   E
      RL   H
      RL   L
      RL   (HL)
      RL   A
      RR   B
      RR   C
      RR   D
      RR   E
      RR   H
      RR   L
      RR   (HL)
      RR   A
      SLA  B
      SLA  C
      SLA  D
      SLA  E
      SLA  H
      SLA  L
      SLA  (HL)
      SLA  A
      SRA  B
      SRA  C
      SRA  D
      SRA  E
      SRA  H
      SRA  L
      SRA  (HL)
      SRA  A
      SRL  B
      SRL  C
      SRL  D
      SRL  E
      SRL  H
      SRL  L
      SRL  (HL)
      SRL  A
      BIT  0,B
      BIT  0,C
      BIT  0,D
      BIT  0,E
      BIT  0,H
      BIT  0,L
      BIT  0,(HL)
      BIT  0,A
      RES  0,B
      RES  0,C
      RES  0,D
      RES  0,E
      RES  0,H
      RES  0,L
      RES  0,(HL)
      RES  0,A
      SET  0,B
      SET  0,C
      SET  0,D
      SET  0,E
      SET  0,H
      SET  0,L
      SET  0,(HL)
      SET  0,A
      RLC  (IX+127)
      RRC  (IX+127)
      RL   (IX+127)
      RR   (IX+127)
      SLA  (IX+127)
      SRA  (IX+127)
      SRL  (IX+127)
      BIT  0,(IX+127)
      RES  0,(IX+127)
      SET  0,(IX+127)
      RLC  (IY-128)
      RRC  (IY-128)
      RL   (IY-128)
      RR   (IY-128)
      SLA  (IY-128)
      SRA  (IY-128)
      SRL  (IY-128)
      BIT  0,(IY-128)
      RES  0,(IY-128)
      SET  0,(IY-128)
