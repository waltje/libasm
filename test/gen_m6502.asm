      BRK
      ORA  ($02,X)
      ORA  $06
      ASL  $07
      PHP
      ORA  #10
      ASL  A
      ORA  >$000E
      ASL  >$000F
      BPL  $0024
      ORA  ($12),Y
      ORA  $16,X
      ASL  $17,X
      CLC
      ORA  >$001A,Y
      ORA  >$001E,X
      ASL  >$001F,X
      JSR  >$0021
      AND  ($22,X)
      BIT  $25
      AND  $26
      ROL  $27
      PLP
      AND  #$2A
      ROL  A
      BIT  >$002D
      AND  >$002E
      ROL  >$002F
      BMI  $006E
      AND  ($32),Y
      AND  $36,X
      ROL  $37,X
      SEC
      AND  >$003A,Y
      AND  >$003E,X
      ROL  >$003F,X
      RTI
      EOR  ($42,X)
      EOR  $46
      LSR  $47
      PHA
      EOR  #$4A
      LSR  A
      JMP  >$004D
      EOR  >$004E
      LSR  >$004F
      BVC  $00B4
      EOR  ($52),Y
      EOR  $56,X
      LSR  $57,X
      CLI
      EOR  >$005A,Y
      EOR  >$005E,X
      LSR  >$005F,X
      RTS
      ADC  ($62,X)
      ADC  $66
      ROR  $67
      PLA
      ADC  #$6A
      ROR  A
      JMP  (>$006D)
      ADC  >$006E
      ROR  >$006F
      BVS  $00FA
      ADC  ($72),Y
      ADC  $76,X
      ROR  $77,X
      SEI
      ADC  >$007A,Y
      ADC  >$007E,X
      ROR  >$007F,X
      STA  ($82,X)
      STY  $85
      STA  $86
      STX  $87
      DEY
      TXA
      STY  >$008D
      STA  >$008E
      STX  >$008F
      BCC  $003F
      STA  ($92),Y
      STY  $95,X
      STA  $96,X
      STX  $97,Y
      TYA
      STA  >$009A,Y
      TXS
      STA  >$009E,X
      LDY  #$A1
      LDA  ($A2,X)
      LDX  #$A3
      LDY  $A5
      LDA  $A6
      LDX  $A7
      TAY
      LDA  #$AA
      TAX
      LDY  >$00AD
      LDA  >$00AE
      LDX  >$00AF
      BCS  $008A
      LDA  ($B2),Y
      LDY  $B5,X
      LDA  $B6,X
      LDX  $B7,Y
      CLV
      LDA  >$00BA,Y
      TSX
      LDY  >$00BD,X
      LDA  >$00BE,X
      LDX  >$00BF,Y
      CPY  #$C1
      CMP  ($C2,X)
      CPY  $C5
      CMP  $C6
      DEC  $C7
      INY
      CMP  #$CA
      DEX
      CPY  >$00CD
      CMP  >$00CE
      DEC  >$00CF
      BNE  $00D9
      CMP  ($D2),Y
      CMP  $D6,X
      DEC  $D7,X
      CLD
      CMP  >$00DA,Y
      CMP  >$00DE,X
      DEC  >$00DF,X
      CPX  #$E1
      SBC  ($E2,X)
      CPX  $E5
      SBC  $E6
      INC  $E7
      INX
      SBC  #$EA
      NOP
      CPX  >$00ED
      SBC  >$00EE
      INC  >$00EF
      BEQ  $0122
      SBC  ($F2),Y
      SBC  $F6,X
      INC  $F7,X
      SED
      SBC  >$00FA,Y
      SBC  >$00FE,X
      INC  >$00FF,X
