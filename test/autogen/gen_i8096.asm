;;; AUTO GENERATED FILE
;;; generated by: gen_i8096 -u -C 8096 -o gen_i8096.asm -l gen_i8096.lst
      CPU   8096
      ORG   2000H
      SKIP  1
      CLR   2
      NOT   4
      NEG   4
      DEC   6
      EXT   8
      INC   8
      SHR   10, #9
      SHR   18, 17
      SHL   12, #11
      SHL   18, 17
      SHRA  12, #11
      SHRA  18, 17
      SHRL  16, #15
      SHRL  20, 19
      SHLL  16, #15
      SHLL  20, 19
      SHRAL 16, #15
      SHRAL 20, 19
      NORML 20, 19
      CLRB  18
      NOTB  19
      NEGB  20
      DECB  22
      EXTB  24
      INCB  24
      SHRB  26, 25
      SHLB  27, 26
      SHRAB 28, 27
      SJMP  $+33
      SJMP  $
      SJMP  $-987
      SCALL $+41
      SCALL $
      SCALL $-979
      JBC   49, 0, $+50
      JBC   49, 0, $-128
      JBC   49, 0, $
      JBS   57, 0, $+58
      JBS   57, 0, $-128
      JBS   57, 0, $
      ANDB  83, 82, 81
      ANDB  84, 83, #52H
      ANDB  86, 85, [84]
      ANDB  87, 86, 85[84]
      ANDB  87, 86, -128[84]
      ANDB  89, 88, 5756H[84]
      ADDB  87, 86, 85
      ADDB  88, 87, #56H
      ADDB  90, 89, [88]
      ADDB  91, 90, 89[88]
      ADDB  91, 90, -128[88]
      ADDB  93, 92, 5B5AH[88]
      SUBB  91, 90, 89
      SUBB  92, 91, #5AH
      SUBB  94, 93, [92]
      SUBB  95, 94, 93[92]
      SUBB  95, 94, -128[92]
      SUBB  97, 96, 5F5EH[92]
      MULUB 96, 95, 94
      MULUB 96, 95, #5EH
      MULUB 98, 97, [96]
      AND   100, #6362H
      AND   102, 101[100]
      AND   102, -128[100]
      AND   104, 6766H[100]
      ADD   104, #6766H
      ADD   106, 105[104]
      ADD   106, -128[104]
      ADD   108, 6B6AH[104]
      SUB   108, #6B6AH
      SUB   110, 109[108]
      SUB   110, -128[108]
      SUB   112, 6F6EH[108]
      MULU  112, #6F6EH
      MULU  116, 7372H[112]
      MULU  116, 115[114]
      MULU  116, -128[114]
      ANDB  114, 113
      ANDB  115, #72H
      ANDB  117, [116]
      ANDB  118, 117[116]
      ANDB  118, -128[116]
      ANDB  120, 7776H[116]
      ADDB  118, 117
      ADDB  119, #76H
      ADDB  121, [120]
      ADDB  122, 121[120]
      ADDB  122, -128[120]
      ADDB  124, 7B7AH[120]
      SUBB  122, 121
      SUBB  123, #7AH
      SUBB  125, [124]
      SUBB  126, 125[124]
      SUBB  126, -128[124]
      SUBB  128, 7F7EH[124]
      MULUB 126, 125
      MULUB 128, #7FH
      MULUB 130, -127[128]
      MULUB 130, 0[128]
      MULUB 132, 8382H[128]
      OR    132, #8382H
      OR    134, -123[132]
      OR    134, 0[132]
      OR    136, 8786H[132]
      XOR   136, #8786H
      XOR   138, -119[136]
      XOR   138, 0[136]
      XOR   140, 8B8AH[136]
      CMP   140, #8B8AH
      CMP   142, -115[140]
      CMP   142, 0[140]
      CMP   144, 8F8EH[140]
      DIVU  144, #8F8EH
      DIVU  148, 9392H[144]
      DIVU  148, -109[146]
      DIVU  148, 0[146]
      ORB   146, 145
      ORB   147, #92H
      ORB   149, [148]
      ORB   150, -107[148]
      ORB   150, 0[148]
      ORB   152, 9796H[148]
      XORB  150, 149
      XORB  151, #96H
      XORB  153, [152]
      XORB  154, -103[152]
      XORB  154, 0[152]
      XORB  156, 9B9AH[152]
      CMPB  154, 153
      CMPB  155, #9AH
      CMPB  157, [156]
      CMPB  158, -99[156]
      CMPB  158, 0[156]
      CMPB  160, 9F9EH[156]
      DIVUB 158, 157
      DIVUB 160, #9FH
      DIVUB 162, -95[160]
      DIVUB 162, 0[160]
      DIVUB 164, 0A3A2H[160]
      LD    164, #0A3A2H
      LD    166, -91[164]
      LD    166, 0[164]
      LD    168, 0A7A6H[164]
      ADDC  168, #0A7A6H
      ADDC  170, -87[168]
      ADDC  170, 0[168]
      ADDC  172, 0ABAAH[168]
      SUBC  172, #0ABAAH
      SUBC  174, -83[172]
      SUBC  174, 0[172]
      SUBC  176, 0AFAEH[172]
      LDBZE 174, 173
      LDBZE 176, #0AFH
      LDBZE 178, -79[176]
      LDBZE 178, 0[176]
      LDBZE 180, 0B3B2H[176]
      LDB   178, 177
      LDB   179, #0B2H
      LDB   181, [180]
      LDB   182, -75[180]
      LDB   182, 0[180]
      LDB   184, 0B7B6H[180]
      ADDCB 182, 181
      ADDCB 183, #0B6H
      ADDCB 185, [184]
      ADDCB 186, -71[184]
      ADDCB 186, 0[184]
      ADDCB 188, 0BBBAH[184]
      SUBCB 186, 185
      SUBCB 187, #0BAH
      SUBCB 189, [188]
      SUBCB 190, -67[188]
      SUBCB 190, 0[188]
      SUBCB 192, 0BFBEH[188]
      LDBSE 190, 189
      LDBSE 192, #0BFH
      LDBSE 194, -63[192]
      LDBSE 194, 0[192]
      LDBSE 196, 0C3C2H[192]
      ST    198, -59[196]
      ST    198, 0[196]
      ST    200, 0C7C6H[196]
      STB   198, 197
      STB   201, [200]
      STB   202, -55[200]
      STB   202, 0[200]
      STB   204, 0CBCAH[200]
      PUSH  202
      PUSH  #0CBCAH
      PUSH  [204]
      PUSH  -51[204]
      PUSH  0[204]
      PUSH  0CFCEH[204]
      POP   206
      POP   [208]
      POP   -47[208]
      POP   0[208]
      POP   0D3D2H[208]
      JNST  $-47
      JNST  $
      JNST  $+1
      JNH   $-46
      JNH   $
      JNH   $+1
      JGT   $-45
      JGT   $
      JGT   $+1
      JNC   $-44
      JNC   $
      JNC   $+1
      JNVT  $-43
      JNVT  $
      JNVT  $+1
      JNV   $-42
      JNV   $
      JNV   $+1
      JGE   $-41
      JGE   $
      JGE   $+1
      JNE   $-40
      JNE   $
      JNE   $+1
      JST   $-39
      JST   $
      JST   $+1
      JH    $-38
      JH    $
      JH    $+1
      JLE   $-37
      JLE   $
      JLE   $+1
      JC    $-36
      JC    $
      JC    $+1
      JVT   $-35
      JVT   $
      JVT   $+1
      JV    $-34
      JV    $
      JV    $+1
      JLT   $-33
      JLT   $
      JLT   $+1
      JE    $-32
      JE    $
      JE    $+1
      DJNZ  225, $-30
      DJNZ  225, $
      DJNZ  225, $+1
      BR    [228]
      LJMP  $-1618H
      LJMP  $+00E8H
      LCALL $-0E10H
      LCALL $+00F0H
      RET
      PUSHF
      POPF
      TRAP
      CLRC
      SETC
      DI
      EI
      CLRVT
      NOP
      MULB  96, 95, #5EH
      MUL   112, #6F6EH
      MULB  126, 125
      MULB  130, -127[128]
      MULB  130, 817FH[126]
      DIV   144, #8F8EH
      DIVB  158, 157
      DIVB  162, -95[160]
      DIVB  162, 0A19FH[158]
      RST
