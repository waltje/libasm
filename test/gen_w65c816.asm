;;; AUTO GENERATED FILE
;;; generated by: gen_mos6502 -u -C 65816 -o gen_w65c816.asm
      CPU 65816
      BRK
      ORA  ($02,X)
      COP  #3
      ORA  $04,S
      TSB  $05
      ORA  $06
      ASL  $07
      ORA  [$08]
      PHP
      ORA  #10
      ASL  A
      PHD
      TSB  >$000D
      TSB  $0100
      ORA  >$000E
      ORA  $0100
      ASL  >$000F
      ASL  $0100
      ORA  >>$001110
      ORA  $010010
      BPL  *+$13
      BPL  *-$30
      BPL  *
      ORA  ($12),Y
      ORA  ($13)
      ORA  ($14,S),Y
      TRB  $15
      ORA  $16,X
      ASL  $17,X
      ORA  [$18],Y
      CLC
      ORA  >$001A,Y
      ORA  $0100,Y
      INC  A
      TCS
      TRB  >$001D
      TRB  $0100
      ORA  >$001E,X
      ORA  $0100,X
      ASL  >$001F,X
      ASL  $0100,X
      ORA  >>$002120,X
      ORA  $010020,X
      JSR  >$0021
      JSR  $0100
      AND  ($22,X)
      JSL  >>$002423
      JSL  $010023
      AND  $24,S
      BIT  $25
      AND  $26
      ROL  $27
      AND  [$28]
      PLP
      AND  #$2A
      ROL  A
      PLD
      BIT  >$002D
      BIT  $0100
      AND  >$002E
      AND  $0100
      ROL  >$002F
      ROL  $0100
      AND  >>$003130
      AND  $010030
      BMI  *+$33
      BMI  *-$7E
      BMI  *
      AND  ($32),Y
      AND  ($33)
      AND  ($34,S),Y
      BIT  $35,X
      AND  $36,X
      ROL  $37,X
      AND  [$38],Y
      SEC
      AND  >$003A,Y
      AND  $0100,Y
      DEC  A
      TSC
      BIT  >$003D,X
      BIT  $0100,X
      AND  >$003E,X
      AND  $0100,X
      ROL  >$003F,X
      ROL  $0100,X
      AND  >>$004140,X
      AND  $010040,X
      RTI
      EOR  ($42,X)
      EOR  $44,S
      MVP  $000000,$450000
      EOR  $46
      LSR  $47
      EOR  [$48]
      PHA
      EOR  #$4A
      LSR  A
      PHK
      JMP  >$004D
      JMP  $0100
      EOR  >$004E
      EOR  $0100
      LSR  >$004F
      LSR  $0100
      EOR  >>$005150
      EOR  $010050
      BVC  *+$53
      BVC  *-$7E
      BVC  *
      EOR  ($52),Y
      EOR  ($53)
      EOR  ($54,S),Y
      MVN  $000000,$550000
      EOR  $56,X
      LSR  $57,X
      EOR  [$58],Y
      CLI
      EOR  >$005A,Y
      EOR  $0100,Y
      PHY
      TCD
      JMP  >>$005E5D
      JMP  $01005D
      EOR  >$005E,X
      EOR  $0100,X
      LSR  >$005F,X
      LSR  $0100,X
      EOR  >>$006160,X
      EOR  $010060,X
      RTS
      ADC  ($62,X)
      PER  *+$0066
      ADC  $64,S
      STZ  $65
      ADC  $66
      ROR  $67
      ADC  [$68]
      PLA
      ADC  #$6A
      ROR  A
      RTL
      JMP  (>$006D)
      JMP  ($0100)
      ADC  >$006E
      ADC  $0100
      ROR  >$006F
      ROR  $0100
      ADC  >>$007170
      ADC  $010070
      BVS  *+$73
      BVS  *-$7E
      BVS  *
      ADC  ($72),Y
      ADC  ($73)
      ADC  ($74,S),Y
      STZ  $75,X
      ADC  $76,X
      ROR  $77,X
      ADC  [$78],Y
      SEI
      ADC  >$007A,Y
      ADC  $0100,Y
      PLY
      TDC
      JMP  (>$007D,X)
      JMP  ($0100,X)
      ADC  >$007E,X
      ADC  $0100,X
      ROR  >$007F,X
      ROR  $0100,X
      ADC  >>$008180,X
      ADC  $010080,X
      BRA  *-$7D
      BRA  *
      BRA  *+1
      STA  ($82,X)
      BRL  *+$0086
      STA  $84,S
      STY  $85
      STA  $86
      STX  $87
      STA  [$88]
      DEY
      BIT  #$8A
      TXA
      PHB
      STY  >$008D
      STY  $0100
      STA  >$008E
      STA  $0100
      STX  >$008F
      STX  $0100
      STA  >>$009190
      STA  $010090
      BCC  *-$6D
      BCC  *
      BCC  *+1
      STA  ($92),Y
      STA  ($93)
      STA  ($94,S),Y
      STY  $95,X
      STA  $96,X
      STX  $97,Y
      STA  [$98],Y
      TYA
      STA  >$009A,Y
      STA  $0100,Y
      TXS
      TXY
      STZ  >$009D
      STZ  $0100
      STA  >$009E,X
      STA  $0100,X
      STZ  >$009F,X
      STZ  $0100,X
      STA  >>$00A1A0,X
      STA  $0100A0,X
      LDY  #$A1
      LDA  ($A2,X)
      LDX  #$A3
      LDA  $A4,S
      LDY  $A5
      LDA  $A6
      LDX  $A7
      LDA  [$A8]
      TAY
      LDA  #$AA
      TAX
      PLB
      LDY  >$00AD
      LDY  $0100
      LDA  >$00AE
      LDA  $0100
      LDX  >$00AF
      LDX  $0100
      LDA  >>$00B1B0
      LDA  $0100B0
      BCS  *-$4D
      BCS  *
      BCS  *+1
      LDA  ($B2),Y
      LDA  ($B3)
      LDA  ($B4,S),Y
      LDY  $B5,X
      LDA  $B6,X
      LDX  $B7,Y
      LDA  [$B8],Y
      CLV
      LDA  >$00BA,Y
      LDA  $0100,Y
      TSX
      TYX
      LDY  >$00BD,X
      LDY  $0100,X
      LDA  >$00BE,X
      LDA  $0100,X
      LDX  >$00BF,Y
      LDX  $0100,Y
      LDA  >>$00C1C0,X
      LDA  $0100C0,X
      CPY  #$C1
      CMP  ($C2,X)
      REP  #$C3
      CMP  $C4,S
      CPY  $C5
      CMP  $C6
      DEC  $C7
      CMP  [$C8]
      INY
      CMP  #$CA
      DEX
      WAI
      CPY  >$00CD
      CPY  $0100
      CMP  >$00CE
      CMP  $0100
      DEC  >$00CF
      DEC  $0100
      CMP  >>$00D1D0
      CMP  $0100D0
      BNE  *-$2D
      BNE  *
      BNE  *+1
      CMP  ($D2),Y
      CMP  ($D3)
      CMP  ($D4,S),Y
      PEI  ($D5)
      CMP  $D6,X
      DEC  $D7,X
      CMP  [$D8],Y
      CLD
      CMP  >$00DA,Y
      CMP  $0100,Y
      PHX
      STP
      JMP  [>$00DD]
      JMP  [$0100]
      CMP  >$00DE,X
      CMP  $0100,X
      DEC  >$00DF,X
      DEC  $0100,X
      CMP  >>$00E1E0,X
      CMP  $0100E0,X
      CPX  #$E1
      SBC  ($E2,X)
      SEP  #$E3
      SBC  $E4,S
      CPX  $E5
      SBC  $E6
      INC  $E7
      SBC  [$E8]
      INX
      SBC  #$EA
      NOP
      XBA
      CPX  >$00ED
      CPX  $0100
      SBC  >$00EE
      SBC  $0100
      INC  >$00EF
      INC  $0100
      SBC  >>$00F1F0
      SBC  $0100F0
      BEQ  *-13
      BEQ  *
      BEQ  *+1
      SBC  ($F2),Y
      SBC  ($F3)
      SBC  ($F4,S),Y
      PEA  >$00F5
      PEA  $0100
      SBC  $F6,X
      INC  $F7,X
      SBC  [$F8],Y
      SED
      SBC  >$00FA,Y
      SBC  $0100,Y
      PLX
      XCE
      JSR  (>$00FD,X)
      JSR  ($0100,X)
      SBC  >$00FE,X
      SBC  $0100,X
      INC  >$00FF,X
      INC  $0100,X
      SBC  $010100,X
