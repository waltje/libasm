;;; AUTO GENERATED FILE
;;; generated by: gen_mc6800 -u -C 6301 -o gen_hd6301.asm
      CPU 6301
      NOP
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
      TAB
      TBA
      XGDX
      DAA
      SLP
      ABA
      BRA   *+35
      BRA   *-126
      BRA   *
      BRN   *+36
      BRN   *-126
      BRN   *
      BHI   *+37
      BHI   *-126
      BHI   *
      BLS   *+38
      BLS   *-126
      BLS   *
      BHS   *+39
      BHS   *-126
      BHS   *
      BLO   *+40
      BLO   *-126
      BLO   *
      BNE   *+41
      BNE   *-126
      BNE   *
      BEQ   *+42
      BEQ   *-126
      BEQ   *
      BVC   *+43
      BVC   *-126
      BVC   *
      BVS   *+44
      BVS   *-126
      BVS   *
      BPL   *+45
      BPL   *-126
      BPL   *
      BMI   *+46
      BMI   *-126
      BMI   *
      BGE   *+47
      BGE   *-126
      BGE   *
      BLT   *+48
      BLT   *-126
      BLT   *
      BGT   *+49
      BGT   *-126
      BGT   *
      BLE   *+50
      BLE   *-126
      BLE   *
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
      AIM   #0,98,X
      OIM   #0,99,X
      BSET  0,0,X
      COM   100,X
      LSR   101,X
      EIM   #0,102,X
      BTGL  0,0,X
      ROR   103,X
      ASR   104,X
      ASL   105,X
      ROL   106,X
      DEC   107,X
      TIM   #0,108,X
      BTST  0,0,X
      INC   109,X
      TST   110,X
      JMP   111,X
      CLR   112,X
      NEG   >$0071
      NEG   $0100
      AIM   #0,$72
      OIM   #0,$73
      BSET  0,$00
      COM   >$0074
      COM   $0100
      LSR   >$0075
      LSR   $0100
      EIM   #0,$76
      BTGL  0,$00
      ROR   >$0077
      ROR   $0100
      ASR   >$0078
      ASR   $0100
      ASL   >$0079
      ASL   $0100
      ROL   >$007A
      ROL   $0100
      DEC   >$007B
      DEC   $0100
      TIM   #0,$7C
      BTST  0,$00
      INC   >$007D
      INC   $0100
      TST   >$007E
      TST   $0100
      JMP   >$007F
      JMP   $0100
      CLR   >$0080
      CLR   $0100
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
      BSR   *-112
      BSR   *
      BSR   *+1
      LDS   #$008F
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
      SUBA  $0100
      CMPA  >$00B2
      CMPA  $0100
      SBCA  >$00B3
      SBCA  $0100
      SUBD  >$00B4
      SUBD  $0100
      ANDA  >$00B5
      ANDA  $0100
      BITA  >$00B6
      BITA  $0100
      LDAA  >$00B7
      LDAA  $0100
      STAA  >$00B8
      STAA  $0100
      EORA  >$00B9
      EORA  $0100
      ADCA  >$00BA
      ADCA  $0100
      ORAA  >$00BB
      ORAA  $0100
      ADDA  >$00BC
      ADDA  $0100
      CPX   >$00BD
      CPX   $0100
      JSR   >$00BE
      JSR   $0100
      LDS   >$00BF
      LDS   $0100
      STS   >$00C0
      STS   $0100
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
      SUBB  $0100
      CMPB  >$00F2
      CMPB  $0100
      SBCB  >$00F3
      SBCB  $0100
      ADDD  >$00F4
      ADDD  $0100
      ANDB  >$00F5
      ANDB  $0100
      BITB  >$00F6
      BITB  $0100
      LDAB  >$00F7
      LDAB  $0100
      STAB  >$00F8
      STAB  $0100
      EORB  >$00F9
      EORB  $0100
      ADCB  >$00FA
      ADCB  $0100
      ORAB  >$00FB
      ORAB  $0100
      ADDB  >$00FC
      ADDB  $0100
      LDD   >$00FD
      LDD   $0100
      STD   >$00FE
      STD   $0100
      LDX   >$00FF
      LDX   $0100
      STX   $0100
