;;; AUTO GENERATED FILE
;;; generated by: gen_z8000 -C z8001 -o gen_z8001.s
      cpu    z8001
      addb   rh0,#1
      addb   rl0,#9
      addb   rh0,@rr2
      addb   rl0,@rr2
      add    r0,#0x0101
      add    r0,@rr2
      subb   rh0,#1
      subb   rl0,#9
      subb   rh0,@rr2
      subb   rl0,@rr2
      sub    r0,#0x0301
      sub    r0,@rr2
      orb    rh0,#1
      orb    rl0,#9
      orb    rh0,@rr2
      orb    rl0,@rr2
      or     r0,#0x0501
      or     r0,@rr2
      andb   rh0,#1
      andb   rl0,#9
      andb   rh0,@rr2
      andb   rl0,@rr2
      and    r0,#0x0701
      and    r0,@rr2
      xorb   rh0,#1
      xorb   rl0,#9
      xorb   rh0,@rr2
      xorb   rl0,@rr2
      xor    r0,#0x0901
      xor    r0,@rr2
      cpb    rh0,#1
      cpb    rl0,#9
      cpb    rh0,@rr2
      cpb    rl0,@rr2
      cp     r0,#0x0b01
      cp     r0,@rr2
      comb   @rr2
      cpb    @rr2,#0x22
      negb   @rr2
      testb  @rr2
      ldb    @rr2,#0x26
      tsetb  @rr2
      clrb   @rr2
      com    @rr2
      cp     @rr2,#0x0d22
      neg    @rr2
      test   @rr2
      ld     @rr2,#0x0d26
      tset   @rr2
      clr    @rr2
      push   @rr2,#0x0d2a
      cpl    rr0,#0x00001001
      cpl    rr0,@rr2
      pushl  @rr2,@rr2
      subl   rr0,#0x00001201
      subl   rr0,@rr2
      push   @rr2,@rr2
      ldl    rr0,#0x00001401
      ldl    rr0,@rr2
      popl   @rr2,@rr2
      addl   rr0,#0x00001601
      addl   rr0,@rr2
      pop    @rr2,@rr2
      multl  rq0,#0x00001801
      multl  rq0,@rr2
      mult   rr0,#0x1901
      mult   rr0,@rr2
      divl   rq0,#0x00001a01
      divl   rq0,@rr2
      div    rr0,#0x1b01
      div    rr0,@rr2
      testl  @rr2
      ldl    @rr2,rr0
      jp     f,@rr2
      jp     lt,@rr2
      jp     le,@rr2
      jp     ule,@rr2
      jp     ov,@rr2
      jp     mi,@rr2
      jp     z,@rr2
      jp     c,@rr2
      jp     @rr2
      jp     ge,@rr2
      jp     gt,@rr2
      jp     ugt,@rr2
      jp     nov,@rr2
      jp     pl,@rr2
      jp     nz,@rr2
      jp     nc,@rr2
      call   @rr2
      ldb    rh0,#1
      ldb    rl0,#9
      ldb    rh0,@rr2
      ldb    rl0,@rr2
      ld     r0,#0x2101
      ld     r0,@rr2
      resb   @rr2,#0
      res    @rr2,#0
      setb   @rr2,#0
      set    @rr2,#0
      bitb   @rr2,#0
      bit    @rr2,#0
      incb   @rr2,#1
      inc    @rr2,#1
      decb   @rr2,#1
      dec    @rr2,#1
      exb    rh0,@rr2
      exb    rl0,@rr2
      ex     r0,@rr2
      ldb    @rr2,rh0
      ldb    @rr2,rl0
      ld     @rr2,r0
      ldrb   rh0,.+0x3005
      ldrb   rl0,.+0x300d
      ldb    rh0,rr2(#0x3021)
      ldb    rl0,rr2(#0x3029)
      ldr    r0,.+0x3105
      ld     r0,rr2(#0x3121)
      ldrb   .+0x3205,rh0
      ldrb   .+0x320d,rl0
      ldb    rr2(#0x3221),rh0
      ldb    rr2(#0x3229),rl0
      ldr    .+0x3305,r0
      ld     rr2(#0x3321),r0
      ldar   rr0,.+0x3405
      lda    rr0,rr2(#0x3421)
      ldrl   rr0,.+0x3505
      ldl    rr0,rr2(#0x3521)
      ldrl   .+0x3705,rr0
      ldl    rr2(#0x3721),rr0
      ldps   @rr2
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
      addb   rh0,0x400001
      addb   rl0,0x400009
      addb   rh0,0x400011(r1)
      addb   rl0,0x400019(r1)
      add    r0,0x410001
      add    r0,0x410011(r1)
      subb   rh0,0x420001
      subb   rl0,0x420009
      subb   rh0,0x420011(r1)
      subb   rl0,0x420019(r1)
      sub    r0,0x430001
      sub    r0,0x430011(r1)
      orb    rh0,0x440001
      orb    rl0,0x440009
      orb    rh0,0x440011(r1)
      orb    rl0,0x440019(r1)
      or     r0,0x450001
      or     r0,0x450011(r1)
      andb   rh0,0x460001
      andb   rl0,0x460009
      andb   rh0,0x460011(r1)
      andb   rl0,0x460019(r1)
      and    r0,0x470001
      and    r0,0x470011(r1)
      xorb   rh0,0x480001
      xorb   rl0,0x480009
      xorb   rh0,0x480011(r1)
      xorb   rl0,0x480019(r1)
      xor    r0,0x490001
      xor    r0,0x490011(r1)
      cpb    rh0,0x4a0001
      cpb    rl0,0x4a0009
      cpb    rh0,0x4a0011(r1)
      cpb    rl0,0x4a0019(r1)
      cp     r0,0x4b0001
      cp     r0,0x4b0011(r1)
      comb   0x4c0001
      cpb    0x000000,#2
      negb   0x4c0003
      testb  0x4c0005
      ldb    0x000000,#6
      tsetb  0x4c0007
      clrb   0x4c0009
      comb   0x4c0011(r1)
      cpb    0x000000(r1),#18
      negb   0x4c0013(r1)
      testb  0x4c0015(r1)
      ldb    0x000000(r1),#22
      tsetb  0x4c0017(r1)
      clrb   0x4c0019(r1)
      com    0x4d0001
      cp     0x000000,#0x4d02
      neg    0x4d0003
      test   0x4d0005
      ld     0x000000,#0x4d06
      tset   0x4d0007
      clr    0x4d0009
      com    0x4d0011(r1)
      cp     0x000000(r1),#0x4d12
      neg    0x4d0013(r1)
      test   0x4d0015(r1)
      ld     0x000000(r1),#0x4d16
      tset   0x4d0017(r1)
      clr    0x4d0019(r1)
      cpl    rr0,0x500001
      cpl    rr0,0x500011(r1)
      pushl  @rr2,0x510021
      pushl  @rr2,0x510022(r1)
      subl   rr0,0x520001
      subl   rr0,0x520011(r1)
      push   @rr2,0x530021
      push   @rr2,0x530022(r1)
      ldl    rr0,0x540001
      ldl    rr0,0x540011(r1)
      popl   0x550021,@rr2
      popl   0x550022(r1),@rr2
      addl   rr0,0x560001
      addl   rr0,0x560011(r1)
      pop    0x570021,@rr2
      pop    0x570022(r1),@rr2
      multl  rq0,0x580001
      multl  rq0,0x580011(r1)
      mult   rr0,0x590001
      mult   rr0,0x590011(r1)
      divl   rq0,0x5a0001
      divl   rq0,0x5a0011(r1)
      div    rr0,0x5b0001
      div    rr0,0x5b0011(r1)
      testl  0x5c0009
      testl  0x5c0019(r1)
      ldl    0x5d0001,rr0
      ldl    0x5d0011(r1),rr0
      jp     f,0x5e0001
      jp     lt,0x5e0002
      jp     le,0x5e0003
      jp     ule,0x5e0004
      jp     ov,0x5e0005
      jp     mi,0x5e0006
      jp     z,0x5e0007
      jp     c,0x5e0008
      jp     0x5e0009
      jp     ge,0x5e000a
      jp     gt,0x5e000b
      jp     ugt,0x5e000c
      jp     nov,0x5e000d
      jp     pl,0x5e000e
      jp     nz,0x5e000f
      jp     nc,0x5e0010
      jp     f,0x5e0011(r1)
      jp     lt,0x5e0012(r1)
      jp     le,0x5e0013(r1)
      jp     ule,0x5e0014(r1)
      jp     ov,0x5e0015(r1)
      jp     mi,0x5e0016(r1)
      jp     z,0x5e0017(r1)
      jp     c,0x5e0018(r1)
      jp     0x5e0019(r1)
      jp     ge,0x5e001a(r1)
      jp     gt,0x5e001b(r1)
      jp     ugt,0x5e001c(r1)
      jp     nov,0x5e001d(r1)
      jp     pl,0x5e001e(r1)
      jp     nz,0x5e001f(r1)
      jp     nc,0x5e0020(r1)
      call   0x5f0001
      call   0x5f0011(r1)
      ldb    rh0,0x600001
      ldb    rl0,0x600009
      ldb    rh0,0x600011(r1)
      ldb    rl0,0x600019(r1)
      ld     r0,0x610001
      ld     r0,0x610011(r1)
      resb   0x620001,#0
      resb   0x620011(r1),#0
      res    0x630001,#0
      res    0x630011(r1),#0
      setb   0x640001,#0
      setb   0x640011(r1),#0
      set    0x650001,#0
      set    0x650011(r1),#0
      bitb   0x660001,#0
      bitb   0x660011(r1),#0
      bit    0x670001,#0
      bit    0x670011(r1),#0
      incb   0x680001,#1
      incb   0x680011(r1),#1
      inc    0x690001,#1
      inc    0x690011(r1),#1
      decb   0x6a0001,#1
      decb   0x6a0011(r1),#1
      dec    0x6b0001,#1
      dec    0x6b0011(r1),#1
      exb    rh0,0x6c0001
      exb    rl0,0x6c0009
      exb    rh0,0x6c0011(r1)
      exb    rl0,0x6c0019(r1)
      ex     r0,0x6d0001
      ex     r0,0x6d0011(r1)
      ldb    0x6e0001,rh0
      ldb    0x6e0009,rl0
      ldb    0x6e0011(r1),rh0
      ldb    0x6e0019(r1),rl0
      ld     0x6f0001,r0
      ld     0x6f0011(r1),r0
      lda    rr0,0x760001
      lda    rr0,0x760011(r1)
      ldps   0x790001
      ldps   0x790011(r1)
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
      pushl  @rr2,rr2
      subl   rr0,rr2
      push   @rr2,r1
      ldl    rr0,rr2
      popl   rr2,@rr2
      addl   rr0,rr2
      pop    r1,@rr2
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
