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
      BRA   $0038
      BRN   $003B
      BHI   $003E
      BLS   $0041
      BHS   $0044
      BLO   $0047
      BNE   $004A
      BEQ   $004D
      BVC   $0050
      BVS   $0053
      BPL   $0056
      BMI   $0059
      BGE   $005C
      BLT   $005F
      BGT   $0062
      BLE   $0065
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
      OIM   #3,0,X
      BSET  2,0,X
      OIM   #5,0,X
      BSET  3,0,X
      OIM   #9,0,X
      COM   100,X
      LSR   101,X
      EIM   #0,102,X
      BTGL  0,0,X
      EIM   #3,0,X
      BTGL  2,0,X
      EIM   #5,0,X
      BTGL  3,0,X
      EIM   #9,0,X
      ROR   103,X
      ASR   104,X
      ASL   105,X
      ROL   106,X
      DEC   107,X
      TIM   #0,108,X
      BTST  0,0,X
      TIM   #3,0,X
      BTST  2,0,X
      TIM   #5,0,X
      BTST  3,0,X
      TIM   #9,0,X
      INC   109,X
      TST   110,X
      JMP   111,X
      CLR   112,X
      NEG   >$0071
      AIM   #0,$72
      OIM   #0,$73
      BSET  0,$00
      OIM   #3,$00
      BSET  2,$00
      OIM   #5,$00
      BSET  3,$00
      OIM   #9,$00
      COM   >$0074
      LSR   >$0075
      EIM   #0,$76
      BTGL  0,$00
      EIM   #3,$00
      BTGL  2,$00
      EIM   #5,$00
      BTGL  3,$00
      EIM   #9,$00
      ROR   >$0077
      ASR   >$0078
      ASL   >$0079
      ROL   >$007A
      DEC   >$007B
      TIM   #0,$7C
      BTST  0,$00
      TIM   #3,$00
      BTST  2,$00
      TIM   #5,$00
      BTST  3,$00
      TIM   #9,$00
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
      BSR   $00C5
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
