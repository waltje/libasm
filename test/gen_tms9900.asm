;;; AUTO GENERATED FILE
;;; generated by: gen_tms9900 -u -C TMS9900 -o gen_tms9900.asm -l gen_tms9900.lst
      CPU  TMS9900
      ORG  0100H
      LI   R0,0102H
      AI   R0,2122H
      ANDI R0,4142H
      ORI  R0,6162H
      CI   R0,8182H
      STWP R0
      STST R0
      LWPI 0E1E2H
      LIMI 0102H
      IDLE
      RSET
      RTWP
      CKON
      CKOF
      LREX
      BLWP R0
      BLWP *R0
      BLWP @2122H
      BLWP @2223H(R1)
      BLWP *R0+
      B    R0
      B    *R0
      B    @6162H
      B    @6263H(R1)
      B    *R0+
      X    R0
      X    *R0
      X    @0A1A2H
      X    @0A2A3H(R1)
      X    *R0+
      CLR  R0
      CLR  *R0
      CLR  @0E1E2H
      CLR  @0E2E3H(R1)
      CLR  *R0+
      NEG  R0
      NEG  *R0
      NEG  @2122H
      NEG  @2223H(R1)
      NEG  *R0+
      INV  R0
      INV  *R0
      INV  @6162H
      INV  @6263H(R1)
      INV  *R0+
      INC  R0
      INC  *R0
      INC  @0A1A2H
      INC  @0A2A3H(R1)
      INC  *R0+
      INCT R0
      INCT *R0
      INCT @0E1E2H
      INCT @0E2E3H(R1)
      INCT *R0+
      DEC  R0
      DEC  *R0
      DEC  @2122H
      DEC  @2223H(R1)
      DEC  *R0+
      DECT R0
      DECT *R0
      DECT @6162H
      DECT @6263H(R1)
      DECT *R0+
      BL   R0
      BL   *R0
      BL   @0A1A2H
      BL   @0A2A3H(R1)
      BL   *R0+
      SWPB R0
      SWPB *R0
      SWPB @0E1E2H
      SWPB @0E2E3H(R1)
      SWPB *R0+
      SETO R0
      SETO *R0
      SETO @2122H
      SETO @2223H(R1)
      SETO *R0+
      ABS  R0
      ABS  *R0
      ABS  @6162H
      ABS  @6263H(R1)
      ABS  *R0+
      SRA  R0,R0
      SRA  R0,1
      SRL  R0,R0
      SRL  R0,1
      SLA  R0,R0
      SLA  R0,1
      SRC  R0,R0
      SRC  R0,1
      JMP  $+2
      JMP  $-254
      JMP  $
      JLT  $+2
      JLT  $-254
      JLT  $
      JLE  $+2
      JLE  $-254
      JLE  $
      JEQ  $+2
      JEQ  $-254
      JEQ  $
      JHE  $+2
      JHE  $-254
      JHE  $
      JGT  $+2
      JGT  $-254
      JGT  $
      JNE  $+2
      JNE  $-254
      JNE  $
      JNC  $+2
      JNC  $-254
      JNC  $
      JOC  $+2
      JOC  $-254
      JOC  $
      JNO  $+2
      JNO  $-254
      JNO  $
      JL   $+2
      JL   $-254
      JL   $
      JH   $+2
      JH   $-254
      JH   $
      JOP  $+2
      JOP  $-254
      JOP  $
      SBO  0
      SBO  -128
      SBZ  0
      SBZ  -128
      TB   0
      TB   -128
      COC  R0,R0
      COC  *R0,R0
      COC  @2122H,R0
      COC  @2223H(R1),R0
      COC  *R0+,R0
      CZC  R0,R0
      CZC  *R0,R0
      CZC  @2122H,R0
      CZC  @2223H(R1),R0
      CZC  *R0+,R0
      XOR  R0,R0
      XOR  *R0,R0
      XOR  @2122H,R0
      XOR  @2223H(R1),R0
      XOR  *R0+,R0
      XOP  R0,0
      XOP  *R0,0
      XOP  @2122H,0
      XOP  @2223H(R1),0
      XOP  *R0+,0
      LDCR R0,16
      LDCR *R0,16
      LDCR @2122H,16
      LDCR @2223H(R1),16
      LDCR *R0+,16
      STCR R0,16
      STCR *R0,16
      STCR @2122H,16
      STCR @2223H(R1),16
      STCR *R0+,16
      MPY  R0,R0
      MPY  *R0,R0
      MPY  @2122H,R0
      MPY  @2223H(R1),R0
      MPY  *R0+,R0
      DIV  R0,R0
      DIV  *R0,R0
      DIV  @2122H,R0
      DIV  @2223H(R1),R0
      DIV  *R0+,R0
      SZC  R0,R0
      SZC  *R0,R0
      SZC  @2122H,R0
      SZC  @2223H(R1),R0
      SZC  *R0+,R0
      SZC  R0,*R0
      SZC  *R0,*R0
      SZC  @2122H,*R0
      SZC  @2223H(R1),*R0
      SZC  *R0+,*R0
      SZC  R0,@0102H
      SZC  *R0,@1112H
      SZC  @2122H,@2324H
      SZC  @2223H(R1),@2425H
      SZC  *R0+,@3132H
      SZC  R0,@4142H(R1)
      SZC  *R0,@5152H(R1)
      SZC  @6162H,@6364H(R1)
      SZC  @6263H(R1),@6465H(R1)
      SZC  *R0+,@7172H(R1)
      SZC  R0,*R0+
      SZC  *R0,*R0+
      SZC  @2122H,*R0+
      SZC  @2223H(R1),*R0+
      SZC  *R0+,*R0+
      SZCB R0,R0
      SZCB *R0,R0
      SZCB @2122H,R0
      SZCB @2223H(R1),R0
      SZCB *R0+,R0
      SZCB R0,*R0
      SZCB *R0,*R0
      SZCB @2122H,*R0
      SZCB @2223H(R1),*R0
      SZCB *R0+,*R0
      SZCB R0,@0102H
      SZCB *R0,@1112H
      SZCB @2122H,@2324H
      SZCB @2223H(R1),@2425H
      SZCB *R0+,@3132H
      SZCB R0,@4142H(R1)
      SZCB *R0,@5152H(R1)
      SZCB @6162H,@6364H(R1)
      SZCB @6263H(R1),@6465H(R1)
      SZCB *R0+,@7172H(R1)
      SZCB R0,*R0+
      SZCB *R0,*R0+
      SZCB @2122H,*R0+
      SZCB @2223H(R1),*R0+
      SZCB *R0+,*R0+
      S    R0,R0
      S    *R0,R0
      S    @2122H,R0
      S    @2223H(R1),R0
      S    *R0+,R0
      S    R0,*R0
      S    *R0,*R0
      S    @2122H,*R0
      S    @2223H(R1),*R0
      S    *R0+,*R0
      S    R0,@0102H
      S    *R0,@1112H
      S    @2122H,@2324H
      S    @2223H(R1),@2425H
      S    *R0+,@3132H
      S    R0,@4142H(R1)
      S    *R0,@5152H(R1)
      S    @6162H,@6364H(R1)
      S    @6263H(R1),@6465H(R1)
      S    *R0+,@7172H(R1)
      S    R0,*R0+
      S    *R0,*R0+
      S    @2122H,*R0+
      S    @2223H(R1),*R0+
      S    *R0+,*R0+
      SB   R0,R0
      SB   *R0,R0
      SB   @2122H,R0
      SB   @2223H(R1),R0
      SB   *R0+,R0
      SB   R0,*R0
      SB   *R0,*R0
      SB   @2122H,*R0
      SB   @2223H(R1),*R0
      SB   *R0+,*R0
      SB   R0,@0102H
      SB   *R0,@1112H
      SB   @2122H,@2324H
      SB   @2223H(R1),@2425H
      SB   *R0+,@3132H
      SB   R0,@4142H(R1)
      SB   *R0,@5152H(R1)
      SB   @6162H,@6364H(R1)
      SB   @6263H(R1),@6465H(R1)
      SB   *R0+,@7172H(R1)
      SB   R0,*R0+
      SB   *R0,*R0+
      SB   @2122H,*R0+
      SB   @2223H(R1),*R0+
      SB   *R0+,*R0+
      C    R0,R0
      C    *R0,R0
      C    @2122H,R0
      C    @2223H(R1),R0
      C    *R0+,R0
      C    R0,*R0
      C    *R0,*R0
      C    @2122H,*R0
      C    @2223H(R1),*R0
      C    *R0+,*R0
      C    R0,@0102H
      C    *R0,@1112H
      C    @2122H,@2324H
      C    @2223H(R1),@2425H
      C    *R0+,@3132H
      C    R0,@4142H(R1)
      C    *R0,@5152H(R1)
      C    @6162H,@6364H(R1)
      C    @6263H(R1),@6465H(R1)
      C    *R0+,@7172H(R1)
      C    R0,*R0+
      C    *R0,*R0+
      C    @2122H,*R0+
      C    @2223H(R1),*R0+
      C    *R0+,*R0+
      CB   R0,R0
      CB   *R0,R0
      CB   @2122H,R0
      CB   @2223H(R1),R0
      CB   *R0+,R0
      CB   R0,*R0
      CB   *R0,*R0
      CB   @2122H,*R0
      CB   @2223H(R1),*R0
      CB   *R0+,*R0
      CB   R0,@0102H
      CB   *R0,@1112H
      CB   @2122H,@2324H
      CB   @2223H(R1),@2425H
      CB   *R0+,@3132H
      CB   R0,@4142H(R1)
      CB   *R0,@5152H(R1)
      CB   @6162H,@6364H(R1)
      CB   @6263H(R1),@6465H(R1)
      CB   *R0+,@7172H(R1)
      CB   R0,*R0+
      CB   *R0,*R0+
      CB   @2122H,*R0+
      CB   @2223H(R1),*R0+
      CB   *R0+,*R0+
      A    R0,R0
      A    *R0,R0
      A    @2122H,R0
      A    @2223H(R1),R0
      A    *R0+,R0
      A    R0,*R0
      A    *R0,*R0
      A    @2122H,*R0
      A    @2223H(R1),*R0
      A    *R0+,*R0
      A    R0,@0102H
      A    *R0,@1112H
      A    @2122H,@2324H
      A    @2223H(R1),@2425H
      A    *R0+,@3132H
      A    R0,@4142H(R1)
      A    *R0,@5152H(R1)
      A    @6162H,@6364H(R1)
      A    @6263H(R1),@6465H(R1)
      A    *R0+,@7172H(R1)
      A    R0,*R0+
      A    *R0,*R0+
      A    @2122H,*R0+
      A    @2223H(R1),*R0+
      A    *R0+,*R0+
      AB   R0,R0
      AB   *R0,R0
      AB   @2122H,R0
      AB   @2223H(R1),R0
      AB   *R0+,R0
      AB   R0,*R0
      AB   *R0,*R0
      AB   @2122H,*R0
      AB   @2223H(R1),*R0
      AB   *R0+,*R0
      AB   R0,@0102H
      AB   *R0,@1112H
      AB   @2122H,@2324H
      AB   @2223H(R1),@2425H
      AB   *R0+,@3132H
      AB   R0,@4142H(R1)
      AB   *R0,@5152H(R1)
      AB   @6162H,@6364H(R1)
      AB   @6263H(R1),@6465H(R1)
      AB   *R0+,@7172H(R1)
      AB   R0,*R0+
      AB   *R0,*R0+
      AB   @2122H,*R0+
      AB   @2223H(R1),*R0+
      AB   *R0+,*R0+
      MOV  R0,R0
      MOV  *R0,R0
      MOV  @2122H,R0
      MOV  @2223H(R1),R0
      MOV  *R0+,R0
      MOV  R0,*R0
      MOV  *R0,*R0
      MOV  @2122H,*R0
      MOV  @2223H(R1),*R0
      MOV  *R0+,*R0
      MOV  R0,@0102H
      MOV  *R0,@1112H
      MOV  @2122H,@2324H
      MOV  @2223H(R1),@2425H
      MOV  *R0+,@3132H
      MOV  R0,@4142H(R1)
      MOV  *R0,@5152H(R1)
      MOV  @6162H,@6364H(R1)
      MOV  @6263H(R1),@6465H(R1)
      MOV  *R0+,@7172H(R1)
      MOV  R0,*R0+
      MOV  *R0,*R0+
      MOV  @2122H,*R0+
      MOV  @2223H(R1),*R0+
      MOV  *R0+,*R0+
      MOVB R0,R0
      MOVB *R0,R0
      MOVB @2122H,R0
      MOVB @2223H(R1),R0
      MOVB *R0+,R0
      MOVB R0,*R0
      MOVB *R0,*R0
      MOVB @2122H,*R0
      MOVB @2223H(R1),*R0
      MOVB *R0+,*R0
      MOVB R0,@0102H
      MOVB *R0,@1112H
      MOVB @2122H,@2324H
      MOVB @2223H(R1),@2425H
      MOVB *R0+,@3132H
      MOVB R0,@4142H(R1)
      MOVB *R0,@5152H(R1)
      MOVB @6162H,@6364H(R1)
      MOVB @6263H(R1),@6465H(R1)
      MOVB *R0+,@7172H(R1)
      MOVB R0,*R0+
      MOVB *R0,*R0+
      MOVB @2122H,*R0+
      MOVB @2223H(R1),*R0+
      MOVB *R0+,*R0+
      SOC  R0,R0
      SOC  *R0,R0
      SOC  @2122H,R0
      SOC  @2223H(R1),R0
      SOC  *R0+,R0
      SOC  R0,*R0
      SOC  *R0,*R0
      SOC  @2122H,*R0
      SOC  @2223H(R1),*R0
      SOC  *R0+,*R0
      SOC  R0,@0102H
      SOC  *R0,@1112H
      SOC  @2122H,@2324H
      SOC  @2223H(R1),@2425H
      SOC  *R0+,@3132H
      SOC  R0,@4142H(R1)
      SOC  *R0,@5152H(R1)
      SOC  @6162H,@6364H(R1)
      SOC  @6263H(R1),@6465H(R1)
      SOC  *R0+,@7172H(R1)
      SOC  R0,*R0+
      SOC  *R0,*R0+
      SOC  @2122H,*R0+
      SOC  @2223H(R1),*R0+
      SOC  *R0+,*R0+
      SOCB R0,R0
      SOCB *R0,R0
      SOCB @2122H,R0
      SOCB @2223H(R1),R0
      SOCB *R0+,R0
      SOCB R0,*R0
      SOCB *R0,*R0
      SOCB @2122H,*R0
      SOCB @2223H(R1),*R0
      SOCB *R0+,*R0
      SOCB R0,@0102H
      SOCB *R0,@1112H
      SOCB @2122H,@2324H
      SOCB @2223H(R1),@2425H
      SOCB *R0+,@3132H
      SOCB R0,@4142H(R1)
      SOCB *R0,@5152H(R1)
      SOCB @6162H,@6364H(R1)
      SOCB @6263H(R1),@6465H(R1)
      SOCB *R0+,@7172H(R1)
      SOCB R0,*R0+
      SOCB *R0,*R0+
      SOCB @2122H,*R0+
      SOCB @2223H(R1),*R0+
      SOCB *R0+,*R0+
