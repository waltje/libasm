;;; AUTO GENERATED FILE
;;; generated by: gen_i8048 -u -C msm80c39 -o gen_msm80c39.asm -l gen_msm80c39.lst
      CPU   MSM80C39
      ORG   100H
      NOP
      HALT
      ADD   A, #4
      JMP   005H
      EN    I
      DEC   A
      IN    A, P1
      MOVD  A, P4
      INC   @R0
      JB    0, 113H
      ADDC  A, #20
      CALL  015H
      DIS   I
      JTF   117H
      INC   A
      INC   R0
      XCH   A, @R0
      MOV   A, #36
      EN    TCNTI
      JNT0  127H
      CLR   A
      XCH   A, R0
      XCHD  A, @R0
      DIS   TCNTI
      JT0   137H
      CPL   A
      OUTL  P1, A
      MOVD  P4, A
      ORL   A, @R0
      MOV   A, T
      ORL   A, #44H
      STRT  CNT
      JNT1  147H
      SWAP  A
      ORL   A, R0
      ANL   A, @R0
      ANL   A, #54H
      STRT  T
      JT1   157H
      DA    A
      ANL   A, R0
      ADD   A, @R0
      MOV   T, A
      MOV   A, P1
      STOP  TCNT
      RRC   A
      ADD   A, R0
      ADDC  A, @R0
      ENT0  CLK
      JF1   177H
      RR    A
      ADDC  A, R0
      MOVX  A, @R0
      HLTS
      RET
      CLR   F0
      JNI   187H
      ORL   P1, #8AH
      ORLD  P4, A
      MOVX  @R0, A
      RETR
      CPL   F0
      JNZ   197H
      CLR   C
      ANL   P1, #9AH
      ANLD  P4, A
      MOV   @R0, A
      FLT
      MOVP  A, @A
      CPL   C
      MOV   R0, A
      MOV   @R0, #177
      JMPP  @A
      JF0   1B7H
      MOV   R0, #185
      DEC   @R0
      FLTT
      MOVP1 P, @R3
      SEL   RB0
      JZ    1C7H
      MOV   A, PSW
      DEC   R0
      XRL   A, @R0
      XRL   A, #0D4H
      MOV   PSW, A
      XRL   A, R0
      DJNZ  @R0, 1E1H
      FRES
      MOVP3 A, @A
      SEL   MB0
      JNC   1E7H
      RL    A
      DJNZ  R0, 1E9H
      MOV   A, @R0
      MOV   P1, @R3
      JC    1F7H
      RLC   A
      MOV   A, R0
