;;; AUTO GENERATED FILE
;;; generated by: gen_mos6502 -u -C W65C02S -o gen_w65c02s.asm
      CPU W65C02S
      BRK
      ORA  ($02,X)
      TSB  $05
      ORA  $06
      ASL  $07
      RMB0 $08
      PHP
      ORA  #10
      ASL  A
      TSB  >$000D
      TSB  $0100
      ORA  >$000E
      ORA  $0100
      ASL  >$000F
      ASL  $0100
      BBR0 $10,*+3
      BPL  *+19
      BPL  *-$26
      BPL  *
      ORA  ($12),Y
      ORA  ($13)
      TRB  $15
      ORA  $16,X
      ASL  $17,X
      RMB1 $18
      CLC
      ORA  >$001A,Y
      ORA  $0100,Y
      INC  A
      TRB  >$001D
      TRB  $0100
      ORA  >$001E,X
      ORA  $0100,X
      ASL  >$001F,X
      ASL  $0100,X
      BBR1 $20,*+3
      JSR  >$0021
      JSR  $0100
      AND  ($22,X)
      BIT  $25
      AND  $26
      ROL  $27
      RMB2 $28
      PLP
      AND  #$2A
      ROL  A
      BIT  >$002D
      BIT  $0100
      AND  >$002E
      AND  $0100
      ROL  >$002F
      ROL  $0100
      BBR2 $30,*+3
      BMI  *+$33
      BMI  *-$7E
      BMI  *
      AND  ($32),Y
      AND  ($33)
      BIT  $35,X
      AND  $36,X
      ROL  $37,X
      RMB3 $38
      SEC
      AND  >$003A,Y
      AND  $0100,Y
      DEC  A
      BIT  >$003D,X
      BIT  $0100,X
      AND  >$003E,X
      AND  $0100,X
      ROL  >$003F,X
      ROL  $0100,X
      BBR3 $40,*+3
      RTI
      EOR  ($42,X)
      EOR  $46
      LSR  $47
      RMB4 $48
      PHA
      EOR  #$4A
      LSR  A
      JMP  >$004D
      JMP  $0100
      EOR  >$004E
      EOR  $0100
      LSR  >$004F
      LSR  $0100
      BBR4 $50,*+3
      BVC  *+$53
      BVC  *-$7E
      BVC  *
      EOR  ($52),Y
      EOR  ($53)
      EOR  $56,X
      LSR  $57,X
      RMB5 $58
      CLI
      EOR  >$005A,Y
      EOR  $0100,Y
      PHY
      EOR  >$005E,X
      EOR  $0100,X
      LSR  >$005F,X
      LSR  $0100,X
      BBR5 $60,*+3
      RTS
      ADC  ($62,X)
      STZ  $65
      ADC  $66
      ROR  $67
      RMB6 $68
      PLA
      ADC  #$6A
      ROR  A
      JMP  (>$006D)
      JMP  ($0100)
      ADC  >$006E
      ADC  $0100
      ROR  >$006F
      ROR  $0100
      BBR6 $70,*+3
      BVS  *+$73
      BVS  *-$7E
      BVS  *
      ADC  ($72),Y
      ADC  ($73)
      STZ  $75,X
      ADC  $76,X
      ROR  $77,X
      RMB7 $78
      SEI
      ADC  >$007A,Y
      ADC  $0100,Y
      PLY
      JMP  (>$007D,X)
      JMP  ($0100,X)
      ADC  >$007E,X
      ADC  $0100,X
      ROR  >$007F,X
      ROR  $0100,X
      BBR7 $80,*+3
      BRA  *-$7D
      BRA  *
      BRA  *+1
      STA  ($82,X)
      STY  $85
      STA  $86
      STX  $87
      SMB0 $88
      DEY
      BIT  #$8A
      TXA
      STY  >$008D
      STY  $0100
      STA  >$008E
      STA  $0100
      STX  >$008F
      STX  $0100
      BBS0 $90,*+3
      BCC  *-$6D
      BCC  *
      BCC  *+1
      STA  ($92),Y
      STA  ($93)
      STY  $95,X
      STA  $96,X
      STX  $97,Y
      SMB1 $98
      TYA
      STA  >$009A,Y
      STA  $0100,Y
      TXS
      STZ  >$009D
      STZ  $0100
      STA  >$009E,X
      STA  $0100,X
      STZ  >$009F,X
      STZ  $0100,X
      BBS1 $A0,*+3
      LDY  #$A1
      LDA  ($A2,X)
      LDX  #$A3
      LDY  $A5
      LDA  $A6
      LDX  $A7
      SMB2 $A8
      TAY
      LDA  #$AA
      TAX
      LDY  >$00AD
      LDY  $0100
      LDA  >$00AE
      LDA  $0100
      LDX  >$00AF
      LDX  $0100
      BBS2 $B0,*+3
      BCS  *-$4D
      BCS  *
      BCS  *+1
      LDA  ($B2),Y
      LDA  ($B3)
      LDY  $B5,X
      LDA  $B6,X
      LDX  $B7,Y
      SMB3 $B8
      CLV
      LDA  >$00BA,Y
      LDA  $0100,Y
      TSX
      LDY  >$00BD,X
      LDY  $0100,X
      LDA  >$00BE,X
      LDA  $0100,X
      LDX  >$00BF,Y
      LDX  $0100,Y
      BBS3 $C0,*+3
      CPY  #$C1
      CMP  ($C2,X)
      CPY  $C5
      CMP  $C6
      DEC  $C7
      SMB4 $C8
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
      BBS4 $D0,*+3
      BNE  *-$2D
      BNE  *
      BNE  *+1
      CMP  ($D2),Y
      CMP  ($D3)
      CMP  $D6,X
      DEC  $D7,X
      SMB5 $D8
      CLD
      CMP  >$00DA,Y
      CMP  $0100,Y
      PHX
      STP
      CMP  >$00DE,X
      CMP  $0100,X
      DEC  >$00DF,X
      DEC  $0100,X
      BBS5 $E0,*+3
      CPX  #$E1
      SBC  ($E2,X)
      CPX  $E5
      SBC  $E6
      INC  $E7
      SMB6 $E8
      INX
      SBC  #$EA
      NOP
      CPX  >$00ED
      CPX  $0100
      SBC  >$00EE
      SBC  $0100
      INC  >$00EF
      INC  $0100
      BBS6 $F0,*+3
      BEQ  *-13
      BEQ  *
      BEQ  *+1
      SBC  ($F2),Y
      SBC  ($F3)
      SBC  $F6,X
      INC  $F7,X
      SMB7 $F8
      SED
      SBC  >$00FA,Y
      SBC  $0100,Y
      PLX
      SBC  >$00FE,X
      SBC  $0100,X
      INC  >$00FF,X
      INC  $0100,X
      BBS7 $00,*+4
