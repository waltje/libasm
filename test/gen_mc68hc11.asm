;;; AUTO GENERATED FILE
;;; generated by: gen_mc6800 -u -C mc68hc11 -o gen_mc68hc11.asm
      CPU 6811
      NOP
      IDIV
      FDIV
      LSRD
      ASLD
      TAP
      TPA
      INX
      DEX
      CLV
      SEV
      CLC
      SEC
      CLI
      SEI
      SBA
      CBA
      BRSET $13,#0,$0029
      BRCLR $14,#0,$002E
      BSET  $00,#$15
      BCLR  $00,#$16
      TAB
      TBA
      DAA
      ABA
      BSET  0,X,#$1D
      BCLR  0,X,#$1E
      BRSET 31,X,#0,$004D
      BRCLR 32,X,#0,$0052
      BRA   $0054
      BRN   $0057
      BHI   $005A
      BLS   $005D
      BHS   $0060
      BLO   $0063
      BNE   $0066
      BEQ   $0069
      BVC   $006C
      BVS   $006F
      BPL   $0072
      BMI   $0075
      BGE   $0078
      BLT   $007B
      BGT   $007E
      BLE   $0081
      TSX
      INS
      PULA
      PULB
      DES
      TXS
      PSHA
      PSHB
      PULX
      RTS
      ABX
      RTI
      PSHX
      MUL
      WAI
      SWI
      NEGA
      COMA
      LSRA
      RORA
      ASRA
      ASLA
      ROLA
      DECA
      INCA
      TSTA
      CLRA
      NEGB
      COMB
      LSRB
      RORB
      ASRB
      ASLB
      ROLB
      DECB
      INCB
      TSTB
      CLRB
      NEG   97,X
      COM   100,X
      LSR   101,X
      ROR   103,X
      ASR   104,X
      ASL   105,X
      ROL   106,X
      DEC   107,X
      INC   109,X
      TST   110,X
      JMP   111,X
      CLR   112,X
      NEG   >$0071
      COM   >$0074
      LSR   >$0075
      ROR   >$0077
      ASR   >$0078
      ASL   >$0079
      ROL   >$007A
      DEC   >$007B
      INC   >$007D
      TST   >$007E
      JMP   >$007F
      CLR   >$0080
      SUBA  #$81
      CMPA  #$82
      SBCA  #$83
      SUBD  #$0084
      ANDA  #$85
      BITA  #$86
      LDAA  #$87
      EORA  #$89
      ADCA  #$8A
      ORAA  #$8B
      ADDA  #$8C
      CPX   #$008D
      BSR   $005D
      LDS   #$008F
      XGDX
      SUBA  $91
      CMPA  $92
      SBCA  $93
      SUBD  $94
      ANDA  $95
      BITA  $96
      LDAA  $97
      STAA  $98
      EORA  $99
      ADCA  $9A
      ORAA  $9B
      ADDA  $9C
      CPX   $9D
      JSR   $9E
      LDS   $9F
      STS   $A0
      SUBA  161,X
      CMPA  162,X
      SBCA  163,X
      SUBD  164,X
      ANDA  165,X
      BITA  166,X
      LDAA  167,X
      STAA  168,X
      EORA  169,X
      ADCA  170,X
      ORAA  171,X
      ADDA  172,X
      CPX   173,X
      JSR   174,X
      LDS   175,X
      STS   176,X
      SUBA  >$00B1
      CMPA  >$00B2
      SBCA  >$00B3
      SUBD  >$00B4
      ANDA  >$00B5
      BITA  >$00B6
      LDAA  >$00B7
      STAA  >$00B8
      EORA  >$00B9
      ADCA  >$00BA
      ORAA  >$00BB
      ADDA  >$00BC
      CPX   >$00BD
      JSR   >$00BE
      LDS   >$00BF
      STS   >$00C0
      SUBB  #$C1
      CMPB  #$C2
      SBCB  #$C3
      ADDD  #$00C4
      ANDB  #$C5
      BITB  #$C6
      LDAB  #$C7
      EORB  #$C9
      ADCB  #$CA
      ORAB  #$CB
      ADDB  #$CC
      LDD   #$00CD
      LDX   #$00CF
      STOP
      SUBB  $D1
      CMPB  $D2
      SBCB  $D3
      ADDD  $D4
      ANDB  $D5
      BITB  $D6
      LDAB  $D7
      STAB  $D8
      EORB  $D9
      ADCB  $DA
      ORAB  $DB
      ADDB  $DC
      LDD   $DD
      STD   $DE
      LDX   $DF
      STX   $E0
      SUBB  225,X
      CMPB  226,X
      SBCB  227,X
      ADDD  228,X
      ANDB  229,X
      BITB  230,X
      LDAB  231,X
      STAB  232,X
      EORB  233,X
      ADCB  234,X
      ORAB  235,X
      ADDB  236,X
      LDD   237,X
      STD   238,X
      LDX   239,X
      STX   240,X
      SUBB  >$00F1
      CMPB  >$00F2
      SBCB  >$00F3
      ADDD  >$00F4
      ANDB  >$00F5
      BITB  >$00F6
      LDAB  >$00F7
      STAB  >$00F8
      EORB  >$00F9
      ADCB  >$00FA
      ORAB  >$00FB
      ADDB  >$00FC
      LDD   >$00FD
      STD   >$00FE
      LDX   >$00FF
      STX   $0100
      INY
      DEY
      BSET  0,Y,#$1D
      BCLR  0,Y,#$1E
      BRSET 31,Y,#0,$0202
      BRCLR 32,Y,#0,$0208
      TSY
      TYS
      PULY
      ABY
      PSHY
      NEG   97,Y
      COM   100,Y
      LSR   101,Y
      ROR   103,Y
      ASR   104,Y
      ASL   105,Y
      ROL   106,Y
      DEC   107,Y
      INC   109,Y
      TST   110,Y
      JMP   111,Y
      CLR   112,Y
      CPY   #$008D
      XGDY
      CPY   $9D
      SUBA  161,Y
      CMPA  162,Y
      SBCA  163,Y
      SUBD  164,Y
      ANDA  165,Y
      BITA  166,Y
      LDAA  167,Y
      STAA  168,Y
      EORA  169,Y
      ADCA  170,Y
      ORAA  171,Y
      ADDA  172,Y
      CPY   173,Y
      JSR   174,Y
      LDS   175,Y
      STS   176,Y
      CPY   >$00BD
      LDY   #$00CF
      LDY   $DF
      STY   $E0
      SUBB  225,Y
      CMPB  226,Y
      SBCB  227,Y
      ADDD  228,Y
      ANDB  229,Y
      BITB  230,Y
      LDAB  231,Y
      STAB  232,Y
      EORB  233,Y
      ADCB  234,Y
      ORAB  235,Y
      ADDB  236,Y
      LDD   237,Y
      STD   238,Y
      LDY   239,Y
      STY   240,Y
      LDY   >$00FF
      STY   $0100
      CPD   #$0084
      CPD   $94
      CPD   164,X
      CPY   173,X
      CPD   >$00B4
      LDY   239,X
      STY   240,X
      CPD   164,Y
      CPX   173,Y
      LDX   239,Y
      STX   240,Y
