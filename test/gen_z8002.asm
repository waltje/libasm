;;; AUTO GENERATED FILE
;;; generated by: gen_z8000 -C Z8002 -o gen_z8002.asm
      CPU Z8002
      addb   rh0,#1
      addb   rl0,#9
      addb   rh0,@r1
      addb   rl0,@r1
      add    r0,#0x0101
      add    r0,@r1
      subb   rh0,#1
      subb   rl0,#9
      subb   rh0,@r1
      subb   rl0,@r1
      sub    r0,#0x0301
      sub    r0,@r1
      orb    rh0,#1
      orb    rl0,#9
      orb    rh0,@r1
      orb    rl0,@r1
      or     r0,#0x0501
      or     r0,@r1
      andb   rh0,#1
      andb   rl0,#9
      andb   rh0,@r1
      andb   rl0,@r1
      and    r0,#0x0701
      and    r0,@r1
      xorb   rh0,#1
      xorb   rl0,#9
      xorb   rh0,@r1
      xorb   rl0,@r1
      xor    r0,#0x0901
      xor    r0,@r1
      cpb    rh0,#1
      cpb    rl0,#9
      cpb    rh0,@r1
      cpb    rl0,@r1
      cp     r0,#0x0b01
      cp     r0,@r1
      comb   @r1
      cpb    @r1,#18
      negb   @r1
      testb  @r1
      ldb    @r1,#22
      tsetb  @r1
      clrb   @r1
      com    @r1
      cp     @r1,#0x0d12
      neg    @r1
      test   @r1
      ld     @r1,#0x0d16
      tset   @r1
      clr    @r1
      push   @r1,#0x0d1a
      cpl    rr0,#0x00001001
      cpl    rr0,@r1
      pushl  @r1,@r1
      subl   rr0,#0x00001201
      subl   rr0,@r1
      push   @r1,@r1
      ldl    rr0,#0x00001401
      ldl    rr0,@r1
      popl   @r1,@r1
      addl   rr0,#0x00001601
      addl   rr0,@r1
      pop    @r1,@r1
      multl  rq0,#0x00001801
      multl  rq0,@r1
      mult   rr0,#0x1901
      mult   rr0,@r1
      divl   rq0,#0x00001a01
      divl   rq0,@r1
      div    rr0,#0x1b01
      div    rr0,@r1
      testl  @r1
      ldl    @r1,rr0
      jp     f,@r1
      jp     lt,@r1
      jp     le,@r1
      jp     ule,@r1
      jp     ov,@r1
      jp     mi,@r1
      jp     z,@r1
      jp     c,@r1
      jp     @r1
      jp     ge,@r1
      jp     gt,@r1
      jp     ugt,@r1
      jp     nov,@r1
      jp     pl,@r1
      jp     nz,@r1
      jp     nc,@r1
      call   @r1
      ldb    rh0,#1
      ldb    rl0,#9
      ldb    rh0,@r1
      ldb    rl0,@r1
      ld     r0,#0x2101
      ld     r0,@r1
      resb   @r1,#0
      res    @r1,#0
      setb   @r1,#0
      set    @r1,#0
      bitb   @r1,#0
      bit    @r1,#0
      incb   @r1,#1
      inc    @r1,#1
      decb   @r1,#1
      dec    @r1,#1
      exb    rh0,@r1
      exb    rl0,@r1
      ex     r0,@r1
      ldb    @r1,rh0
      ldb    @r1,rl0
      ld     @r1,r0
      ldrb   rh0,.+0x3005
      ldrb   rl0,.+0x300d
      ldb    rh0,r1(#0x3011)
      ldb    rl0,r1(#0x3019)
      ldr    r0,.+0x3105
      ld     r0,r1(#0x3111)
      ldrb   .+0x3205,rh0
      ldrb   .+0x320d,rl0
      ldb    r1(#0x3211),rh0
      ldb    r1(#0x3219),rl0
      ldr    .+0x3305,r0
      ld     r1(#0x3311),r0
      ldar   r0,.+0x3405
      lda    r0,r1(#0x3411)
      ldrl   rr0,.+0x3505
      ldl    rr0,r1(#0x3511)
      ldrl   .+0x3705,rr0
      ldl    r1(#0x3711),rr0
      ldps   @r1
      inb    rh0,#0x3a05
      sinb   rh0,#0x3a06
      outb   #0x3a07,rh0
      soutb  #0x3a08,rh0
      inb    rl0,#0x3a85
      sinb   rl0,#0x3a86
      outb   #0x3a87,rl0
      soutb  #0x3a88,rl0
      in     r0,#0x3b05
      sin    r0,#0x3b06
      out    #0x3b07,r0
      sout   #0x3b08,r0
      inb    rh0,@r1
      inb    rl0,@r1
      in     r0,@r1
      outb   @r1,rh0
      outb   @r1,rl0
      out    @r1,r0
      addb   rh0,0x4001
      addb   rl0,0x4009
      addb   rh0,0x4011(r1)
      addb   rl0,0x4019(r1)
      add    r0,0x4101
      add    r0,0x4111(r1)
      subb   rh0,0x4201
      subb   rl0,0x4209
      subb   rh0,0x4211(r1)
      subb   rl0,0x4219(r1)
      sub    r0,0x4301
      sub    r0,0x4311(r1)
      orb    rh0,0x4401
      orb    rl0,0x4409
      orb    rh0,0x4411(r1)
      orb    rl0,0x4419(r1)
      or     r0,0x4501
      or     r0,0x4511(r1)
      andb   rh0,0x4601
      andb   rl0,0x4609
      andb   rh0,0x4611(r1)
      andb   rl0,0x4619(r1)
      and    r0,0x4701
      and    r0,0x4711(r1)
      xorb   rh0,0x4801
      xorb   rl0,0x4809
      xorb   rh0,0x4811(r1)
      xorb   rl0,0x4819(r1)
      xor    r0,0x4901
      xor    r0,0x4911(r1)
      cpb    rh0,0x4a01
      cpb    rl0,0x4a09
      cpb    rh0,0x4a11(r1)
      cpb    rl0,0x4a19(r1)
      cp     r0,0x4b01
      cp     r0,0x4b11(r1)
      comb   0x4c01
      cpb    0x0000,#2
      negb   0x4c03
      testb  0x4c05
      ldb    0x0000,#6
      tsetb  0x4c07
      clrb   0x4c09
      comb   0x4c11(r1)
      cpb    0x0000(r1),#18
      negb   0x4c13(r1)
      testb  0x4c15(r1)
      ldb    0x0000(r1),#22
      tsetb  0x4c17(r1)
      clrb   0x4c19(r1)
      com    0x4d01
      cp     0x0000,#0x4d02
      neg    0x4d03
      test   0x4d05
      ld     0x0000,#0x4d06
      tset   0x4d07
      clr    0x4d09
      com    0x4d11(r1)
      cp     0x0000(r1),#0x4d12
      neg    0x4d13(r1)
      test   0x4d15(r1)
      ld     0x0000(r1),#0x4d16
      tset   0x4d17(r1)
      clr    0x4d19(r1)
      cpl    rr0,0x5001
      cpl    rr0,0x5011(r1)
      pushl  @r1,0x5111
      pushl  @r1,0x5112(r1)
      subl   rr0,0x5201
      subl   rr0,0x5211(r1)
      push   @r1,0x5311
      push   @r1,0x5312(r1)
      ldl    rr0,0x5401
      ldl    rr0,0x5411(r1)
      popl   0x5511,@r1
      popl   0x5512(r1),@r1
      addl   rr0,0x5601
      addl   rr0,0x5611(r1)
      pop    0x5711,@r1
      pop    0x5712(r1),@r1
      multl  rq0,0x5801
      multl  rq0,0x5811(r1)
      mult   rr0,0x5901
      mult   rr0,0x5911(r1)
      divl   rq0,0x5a01
      divl   rq0,0x5a11(r1)
      div    rr0,0x5b01
      div    rr0,0x5b11(r1)
      testl  0x5c09
      testl  0x5c19(r1)
      ldl    0x5d01,rr0
      ldl    0x5d11(r1),rr0
      jp     f,0x5e01
      jp     lt,0x5e02
      jp     le,0x5e03
      jp     ule,0x5e04
      jp     ov,0x5e05
      jp     mi,0x5e06
      jp     z,0x5e07
      jp     c,0x5e08
      jp     0x5e09
      jp     ge,0x5e0a
      jp     gt,0x5e0b
      jp     ugt,0x5e0c
      jp     nov,0x5e0d
      jp     pl,0x5e0e
      jp     nz,0x5e0f
      jp     nc,0x5e10
      jp     f,0x5e11(r1)
      jp     lt,0x5e12(r1)
      jp     le,0x5e13(r1)
      jp     ule,0x5e14(r1)
      jp     ov,0x5e15(r1)
      jp     mi,0x5e16(r1)
      jp     z,0x5e17(r1)
      jp     c,0x5e18(r1)
      jp     0x5e19(r1)
      jp     ge,0x5e1a(r1)
      jp     gt,0x5e1b(r1)
      jp     ugt,0x5e1c(r1)
      jp     nov,0x5e1d(r1)
      jp     pl,0x5e1e(r1)
      jp     nz,0x5e1f(r1)
      jp     nc,0x5e20(r1)
      call   0x5f01
      call   0x5f11(r1)
      ldb    rh0,0x6001
      ldb    rl0,0x6009
      ldb    rh0,0x6011(r1)
      ldb    rl0,0x6019(r1)
      ld     r0,0x6101
      ld     r0,0x6111(r1)
      resb   0x6201,#0
      resb   0x6211(r1),#0
      res    0x6301,#0
      res    0x6311(r1),#0
      setb   0x6401,#0
      setb   0x6411(r1),#0
      set    0x6501,#0
      set    0x6511(r1),#0
      bitb   0x6601,#0
      bitb   0x6611(r1),#0
      bit    0x6701,#0
      bit    0x6711(r1),#0
      incb   0x6801,#1
      incb   0x6811(r1),#1
      inc    0x6901,#1
      inc    0x6911(r1),#1
      decb   0x6a01,#1
      decb   0x6a11(r1),#1
      dec    0x6b01,#1
      dec    0x6b11(r1),#1
      exb    rh0,0x6c01
      exb    rl0,0x6c09
      exb    rh0,0x6c11(r1)
      exb    rl0,0x6c19(r1)
      ex     r0,0x6d01
      ex     r0,0x6d11(r1)
      ldb    0x6e01,rh0
      ldb    0x6e09,rl0
      ldb    0x6e11(r1),rh0
      ldb    0x6e19(r1),rl0
      ld     0x6f01,r0
      ld     0x6f11(r1),r0
      lda    r0,0x7601
      lda    r0,0x7611(r1)
      ldps   0x7901
      ldps   0x7911(r1)
      halt
      iret
      mset
      mres
      mbit
      mreq   r0
      di     vi,nvi
      di     vi
      di     nvi
      ei     vi,nvi
      ei     vi
      ei     nvi
      ldctl  r0,fcw
      ldctl  r0,refresh
      ldctl  r0,psapseg
      ldctl  r0,psapoff
      ldctl  r0,nspseg
      ldctl  r0,nspoff
      ldctl  fcw,r0
      ldctl  refresh,r0
      ldctl  psapseg,r0
      ldctl  psapoff,r0
      ldctl  nspseg,r0
      ldctl  nspoff,r0
      sc     #0
      addb   rh0,rh1
      addb   rl0,rh1
      addb   rh0,rl0
      addb   rl0,rl0
      add    r0,r1
      subb   rh0,rh1
      subb   rl0,rh1
      subb   rh0,rl0
      subb   rl0,rl0
      sub    r0,r1
      orb    rh0,rh1
      orb    rl0,rh1
      orb    rh0,rl0
      orb    rl0,rl0
      or     r0,r1
      andb   rh0,rh1
      andb   rl0,rh1
      andb   rh0,rl0
      andb   rl0,rl0
      and    r0,r1
      xorb   rh0,rh1
      xorb   rl0,rh1
      xorb   rh0,rl0
      xorb   rl0,rl0
      xor    r0,r1
      cpb    rh0,rh1
      cpb    rl0,rh1
      cpb    rh0,rl0
      cpb    rl0,rl0
      cp     r0,r1
      ldctlb rh0,flags
      ldctlb flags,rh0
      comb   rh1
      negb   rh1
      testb  rh1
      tsetb  rh1
      clrb   rh1
      comb   rl0
      ldctlb rl0,flags
      negb   rl0
      testb  rl0
      tsetb  rl0
      clrb   rl0
      ldctlb flags,rl0
      nop
      com    r1
      setflg p
      neg    r1
      resflg p
      test   r1
      comflg p
      tset   r1
      clr    r1
      setflg s
      resflg s
      comflg s
      setflg s,p
      resflg s,p
      comflg s,p
      setflg z
      resflg z
      comflg z
      setflg z,p
      resflg z,p
      comflg z,p
      setflg z,s
      resflg z,s
      comflg z,s
      setflg z,s,p
      resflg z,s,p
      comflg z,s,p
      setflg c
      resflg c
      comflg c
      setflg c,p
      resflg c,p
      comflg c,p
      setflg c,s
      resflg c,s
      comflg c,s
      setflg c,s,p
      resflg c,s,p
      comflg c,s,p
      setflg c,z
      resflg c,z
      comflg c,z
      setflg c,z,p
      resflg c,z,p
      comflg c,z,p
      setflg c,z,s
      resflg c,z,s
      comflg c,z,s
      setflg c,z,s,p
      resflg c,z,s,p
      comflg c,z,s,p
      cpl    rr0,rr2
      pushl  @r1,rr2
      subl   rr0,rr2
      push   @r1,r1
      ldl    rr0,rr2
      popl   rr2,@r1
      addl   rr0,rr2
      pop    r1,@r1
      multl  rq0,rr2
      mult   rr0,r1
      divl   rq0,rr2
      div    rr0,r1
      testl  rr2
      ret    f
      ret    lt
      ret    le
      ret    ule
      ret    ov
      ret    mi
      ret    z
      ret    c
      ret
      ret    ge
      ret    gt
      ret    ugt
      ret    nov
      ret    pl
      ret    nz
      ret    nc
      ldb    rh0,rh1
      ldb    rl0,rh1
      ldb    rh0,rl0
      ldb    rl0,rl0
      ld     r0,r1
      resb   rh1,#0
      resb   rl0,#0
      res    r1,#0
      setb   rh1,#0
      setb   rl0,#0
      set    r1,#0
      bitb   rh1,#0
      bitb   rl0,#0
      bit    r1,#0
      incb   rh1,#1
      incb   rl0,#1
      inc    r1,#1
      decb   rh1,#1
      decb   rl0,#1
      dec    r1,#1
      exb    rh0,rh1
      exb    rl0,rh1
      exb    rh0,rl0
      exb    rl0,rl0
      ex     r0,r1
      tccb   f,rh0
      tccb   lt,rh0
      tccb   le,rh0
      tccb   ule,rh0
      tccb   ov,rh0
      tccb   mi,rh0
      tccb   z,rh0
      tccb   c,rh0
      tccb   rh0
      tccb   ge,rh0
      tccb   gt,rh0
      tccb   ugt,rh0
      tccb   nov,rh0
      tccb   pl,rh0
      tccb   nz,rh0
      tccb   nc,rh0
      tccb   f,rl0
      tccb   lt,rl0
      tccb   le,rl0
      tccb   ule,rl0
      tccb   ov,rl0
      tccb   mi,rl0
      tccb   z,rl0
      tccb   c,rl0
      tccb   rl0
      tccb   ge,rl0
      tccb   gt,rl0
      tccb   ugt,rl0
      tccb   nov,rl0
      tccb   pl,rl0
      tccb   nz,rl0
      tccb   nc,rl0
      tcc    f,r0
      tcc    lt,r0
      tcc    le,r0
      tcc    ule,r0
      tcc    ov,r0
      tcc    mi,r0
      tcc    z,r0
      tcc    c,r0
      tcc    r0
      tcc    ge,r0
      tcc    gt,r0
      tcc    ugt,r0
      tcc    nov,r0
      tcc    pl,r0
      tcc    nz,r0
      tcc    nc,r0
      dab    rh0
      dab    rl0
      extsb  r0
      extsl  rq0
      exts   rr0
      rlb    rh0,#1
      rrb    rh0,#1
      rlcb   rh0,#1
      rrcb   rh0,#1
      rlb    rl0,#1
      rrb    rl0,#1
      rlcb   rl0,#1
      rrcb   rl0,#1
      rl     r0,#1
      rr     r0,#1
      rlc    r0,#1
      rrc    r0,#1
      adcb   rh0,rh0
      adcb   rl0,rh0
      adcb   rh0,rl0
      adcb   rl0,rl0
      adc    r0,r0
      sbcb   rh0,rh0
      sbcb   rl0,rh0
      sbcb   rh0,rl0
      sbcb   rl0,rl0
      sbc    r0,r0
      rrdb   rh0,rh0
      rrdb   rl0,rh0
      rrdb   rh0,rl0
      rrdb   rl0,rl0
      ldk    r0,#0
      rldb   rh0,rh0
      rldb   rl0,rh0
      rldb   rh0,rl0
      rldb   rl0,rl0
      ldb    rh0,#0
