;;; AUTO GENERATED FILE
;;; generated by: gen_mos6502 -u -C r65c02 -o gen_r65c02.asm

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
      ORA  >$000E
      ASL  >$000F
      BBR0 $10,$001B
      BPL  $002E
      ORA  ($12),Y
      ORA  ($13)
      TRB  $15
      ORA  $16,X
      ASL  $17,X
      RMB1 $18
      CLC
      ORA  >$001A,Y
      INC  A
      TRB  >$001D
      ORA  >$001E,X
      ASL  >$001F,X
      BBR1 $20,$003A
      JSR  >$0021
      AND  ($22,X)
      BIT  $25
      AND  $26
      ROL  $27
      RMB2 $28
      PLP
      AND  #$2A
      ROL  A
      BIT  >$002D
      AND  >$002E
      ROL  >$002F
      BBR2 $30,$0057
      BMI  $008A
      AND  ($32),Y
      AND  ($33)
      BIT  $35,X
      AND  $36,X
      ROL  $37,X
      RMB3 $38
      SEC
      AND  >$003A,Y
      DEC  A
      BIT  >$003D,X
      AND  >$003E,X
      ROL  >$003F,X
      BBR3 $40,$0076
      RTI
      EOR  ($42,X)
      EOR  $46
      LSR  $47
      RMB4 $48
      PHA
      EOR  #$4A
      LSR  A
      JMP  >$004D
      EOR  >$004E
      LSR  >$004F
      BBR4 $50,$008F
      BVC  $00E2
      EOR  ($52),Y
      EOR  ($53)
      EOR  $56,X
      LSR  $57,X
      RMB5 $58
      CLI
      EOR  >$005A,Y
      PHY
      EOR  >$005E,X
      LSR  >$005F,X
      BBR5 $60,$00A9
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
      ADC  >$006E
      ROR  >$006F
      BBR6 $70,$00C4
      BVS  $0137
      ADC  ($72),Y
      ADC  ($73)
      STZ  $75,X
      ADC  $76,X
      ROR  $77,X
      RMB7 $78
      SEI
      ADC  >$007A,Y
      PLY
      JMP  (>$007D,X)
      ADC  >$007E,X
      ROR  >$007F,X
      BBR7 $80,$00E3
      BRA  $0066
      STA  ($82,X)
      STY  $85
      STA  $86
      STX  $87
      SMB0 $88
      DEY
      BIT  #$8A
      TXA
      STY  >$008D
      STA  >$008E
      STX  >$008F
      BBS0 $90,$00FF
      BCC  $0092
      STA  ($92),Y
      STA  ($93)
      STY  $95,X
      STA  $96,X
      STX  $97,Y
      SMB1 $98
      TYA
      STA  >$009A,Y
      TXS
      STZ  >$009D
      STA  >$009E,X
      STZ  >$009F,X
      BBS1 $A0,$011E
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
      LDA  >$00AE
      LDX  >$00AF
      BBS2 $B0,$013C
      BCS  $00EF
      LDA  ($B2),Y
      LDA  ($B3)
      LDY  $B5,X
      LDA  $B6,X
      LDX  $B7,Y
      SMB3 $B8
      CLV
      LDA  >$00BA,Y
      TSX
      LDY  >$00BD,X
      LDA  >$00BE,X
      LDX  >$00BF,Y
      BBS3 $C0,$015B
      CPY  #$C1
      CMP  ($C2,X)
      CPY  $C5
      CMP  $C6
      DEC  $C7
      SMB4 $C8
      INY
      CMP  #$CA
      DEX
      CPY  >$00CD
      CMP  >$00CE
      DEC  >$00CF
      BBS4 $D0,$0177
      BNE  $014A
      CMP  ($D2),Y
      CMP  ($D3)
      CMP  $D6,X
      DEC  $D7,X
      SMB5 $D8
      CLD
      CMP  >$00DA,Y
      PHX
      CMP  >$00DE,X
      DEC  >$00DF,X
      BBS5 $E0,$0191
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
      SBC  >$00EE
      INC  >$00EF
      BBS6 $F0,$01AD
      BEQ  $01A0
      SBC  ($F2),Y
      SBC  ($F3)
      SBC  $F6,X
      INC  $F7,X
      SMB7 $F8
      SED
      SBC  >$00FA,Y
      PLX
      SBC  >$00FE,X
      INC  >$00FF,X
      BBS7 $00,$01C8
