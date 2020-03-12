;;; Copyright 2020 Tadashi G. Takaoka
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at
;;;
;;;     http://www.apache.org/licenses/LICENSE-2.0
;;;
;;; Unless required by applicable law or agreed to in writing, software
;;; distributed under the License is distributed on an "AS IS" BASIS,
;;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;;; See the License for the specific language governing permissions and
;;; limitations under the License.

;;; i8080
        cpu     z80

;;; 0X
        nop                     ;comment
        ld      bc,0302H
        ld      (bc),a
        inc     bc
        inc     b
        dec     b
        ld      b,07H
        rlca
        add     hl,bc
        ld      a,(bc)
        dec     bc
        inc     c
        dec     c
        ld      c,0fH
        rrca
;;; 1X
        ld      de,1312H
        ld      (de),a
        inc     de
        inc     d
        dec     d
        ld      d,17H
        rla
        add     hl,de
        ld      a,(de)
        dec     de
        inc     e
        dec     e
        ld      e,1fH
        rra
;;; 2X
        ld      hl,2322H
        ld      (2423H),hl
        inc     hl
        inc     h
        dec     h
        ld      h,27H
        daa
        add     hl,hl
        ld      hl,(2c2bH)
        dec     hl
        inc     l
        dec     l
        ld      l,2fH
        cpl
;;; 3X
        ld      sp,3332H
        ld      (3433H),a
        inc     sp
        inc     (hl)
        dec     (hl)
        ld      (hl),37H
        scf
        add     hl,sp
        ld      a,(3c3bH)
        dec     sp
        inc     a
        dec     a
        ld      a,3fH
        ccf
;;; 4X
        ld      b,b
        ld      b,c
        ld      b,d
        ld      b,e
        ld      b,h
        ld      b,l
        ld      b,(hl)
        ld      b,a
        ld      c,b
        ld      c,c
        ld      c,d
        ld      c,e
        ld      c,h
        ld      c,l
        ld      c,(hl)
        ld      c,a
;;; 5X
        ld      d,b
        ld      d,c
        ld      d,d
        ld      d,e
        ld      d,h
        ld      d,l
        ld      d,(hl)
        ld      d,a
        ld      e,b
        ld      e,c
        ld      e,d
        ld      e,e
        ld      e,h
        ld      e,l
        ld      e,(hl)
        ld      e,a
;;; 6X
        ld      h,b
        ld      h,c
        ld      h,d
        ld      h,e
        ld      h,h
        ld      h,l
        ld      h,(hl)
        ld      h,a
        ld      l,b
        ld      l,c
        ld      l,d
        ld      l,e
        ld      l,h
        ld      l,l
        ld      l,(hl)
        ld      l,a
;;; 7X
        ld      (hl),b
        ld      (hl),c
        ld      (hl),d
        ld      (hl),e
        ld      (hl),h
        ld      (hl),l
        halt
        ld      (hl),a
        ld      a,b
        ld      a,c
        ld      a,d
        ld      a,e
        ld      a,h
        ld      a,l
        ld      a,(hl)
        ld      a,a
;;; 8X
        add     a,b
        add     a,c
        add     a,d
        add     a,e
        add     a,h
        add     a,l
        add     a,(hl)
        add     a,a
        adc     a,b
        adc     a,c
        adc     a,d
        adc     a,e
        adc     a,h
        adc     a,l
        adc     a,(hl)
        adc     a,a
;;; 9X
        sub     b
        sub     c
        sub     d
        sub     e
        sub     h
        sub     l
        sub     (hl)
        sub     a
        sbc     a,b
        sbc     a,c
        sbc     a,d
        sbc     a,e
        sbc     a,h
        sbc     a,l
        sbc     a,(hl)
        sbc     a,a
;;; AX
        and     b
        and     c
        and     d
        and     e
        and     h
        and     l
        and     (hl)
        and     a
        xor     b
        xor     c
        xor     d
        xor     e
        xor     h
        xor     l
        xor     (hl)
        xor     a
;;; BX
        or      b
        or      c
        or      d
        or      e
        or      h
        or      l
        or      (hl)
        or      a
        cp      b
        cp      c
        cp      d
        cp      e
        cp      h
        cp      l
        cp      (hl)
        cp      a
;;; CX
        ret     nz
        pop     bc
        jp      nz,0c4c3H
        jp      0c5c4H
        call    nz,0c6c5H
        push    bc
        add     a,0c7H
        rst     00h
        ret     z
        ret
        jp      z,0cccbH
        call    z,0cecdH
        call    0cfceH
        adc     a,0cfH
        rst     08H
;;; DX
        ret     nc
        pop     de
        jp      nc,0d4d3H
        out     (0d4h),a
        call    nc,0d6d5H
        push    de
        sub     0d7H
        rst     10h
        ret     c
        jp      c,0dcdbH
        in      a,(0dcH)
        call    c,0deddH
        sbc     a,0dfH
        rst     18H
;;; EX
        ret     po
        pop     hl
        jp      po,0e4e3H
        ex      (sp),hl
        call    po,0e6e5H
        push    hl
        and     0e7H
        rst     20H
        ret     pe
        jp      (hl)
        jp      pe,0ecebH
        ex      de,hl
        call    pe,0eeedH
        xor     0efH
        rst     28H
;;; FX
        ret     p
        pop     af
        jp      p,0f4f3H
        di
        call    p,0f6f5H
        push    af
        or      0f7H
        rst     30H
        ret     m
        ld      sp,hl
        jp      m,0fcfbH
        ei
        call    m,0fefdH
        cp      0ffH
        rst     38H

;;; Z80
        cpu     z80
        org     0140H

;;; 0X
        ex      af,af'
;;; 1X
        djnz    $+10H
        jr      $+18H
;;; 2X
        jr      nz,$+20H
        jr      z,$+28H
;;; 3X
        jr      nc,$+30H
        jr      c,$+38H
;;; DX
        exx

;;; ED4X
        in      b,(c)
        out     (c),b
        sbc     hl,bc
        ld      (0ed43H),bc
        neg
        retn
        im      0
        ld      i,a
        in      c,(c)
        out     (c),c
        adc     hl,bc
        ld      bc,(0ed4bH)
        reti
        ld      r,a
;;; ED5X
        in      d,(c)
        out     (c),d
        sbc     hl,de
        ld      (0ed53H),de
        im      1
        ld      a,i
        in      e,(c)
        out     (c),e
        adc     hl,de
        ld      de,(0ed5bH)
        im      2
        ld      a,r
;;; ED6X
        in      h,(c)
        out     (c),h
        sbc     hl,hl
        rrd
        in      l,(c)
        out     (c),l
        adc     hl,hl
        rld
;;; ED7X
        sbc     hl,sp
        ld      (0ed73H),sp
        in      a,(c)
        out     (c),a
        adc     hl,sp
        ld      sp,(0ed7bH)
;;; EDAX
        ldi
        cpi
        ini
        outi
        ldd
        cpd
        ind
        outd
;;; ED*X
        ldir
        cpir
        inir
        otir
        lddr
        cpdr
        indr
        otdr

;;; DD0X
        add     ix,bc
;;; DD1X
        add     ix,de
;;; DD2X
        ld      ix,0dd21H
        ld      (0dd22H),ix
        inc     ix
        add     ix,ix
        ld      ix,(0dd2aH)
        dec     ix
;;; DD3X
        inc     (ix+35H)
        dec     (ix+36H)
        ld      (ix+37H),0ddH
        add     ix,sp
;;; DD4X
        ld      b,(ix+47H)
        ld      c,(ix+4fH)
;;; DD5X
        ld      d,(ix+57H)
        ld      e,(ix+5fH)
;;; DD6X
        ld      h,(ix+67H)
        ld      l,(ix+6fH)
;;; DD7X
        ld      (ix+71H),b
        ld      (ix+72H),c
        ld      (ix+73H),d
        ld      (ix+74H),e
        ld      (ix+75H),h
        ld      (ix+76H),l
        ld      (ix+78H),a
        ld      a,(ix+7fH)
;;; DD8X
        add     a,(ix-121)
        adc     a,(ix-114)
;;; DD9X
        sub     (ix-105)
        sbc     a,(ix-97)
;;; DDAX
        and     (ix-89)
        xor     (ix-81)
;;; DDBX
        or      (ix-73)
        cp      (ix-65)
;;; DDEX
        pop     ix
        ex      (sp),ix
        push    ix
        jp      (ix)
;;; DDFX
        ld      sp,ix

;;; FD0X
        add     iy,bc
;;; FD1X
        add     iy,de
;;; FD2X
        ld      iy,0fd21H
        ld      (0fd22H),iy
        inc     iy
        add     iy,iy
        ld      iy,(0fd2aH)
        dec     iy
;;; FD3X
        inc     (iy+35H)
        dec     (iy+36H)
        ld      (iy+37H),0fdH
        add     iy,sp
;;; FD4X
        ld      b,(iy+47H)
        ld      c,(iy+4fH)
;;; FD5X
        ld      d,(iy+57H)
        ld      e,(iy+5fH)
;;; FD6X
        ld      h,(iy+67H)
        ld      l,(iy+6fH)
;;; FD7X
        ld      (iy+71H),b
        ld      (iy+72H),c
        ld      (iy+73H),d
        ld      (iy+74H),e
        ld      (iy+75H),h
        ld      (iy+76H),l
        ld      (iy+78H),a
        ld      a,(iy+7fH)
;;; FD8X
        add     a,(iy-121)
        adc     a,(iy-113)
;;; FD9X
        sub     (iy-105)
        sbc     a,(iy-97)
;;; FDAX
        and     (iy-89)
        xor     (iy-81)
;;; FDBX
        or      (iy-73)
        cp      (iy-65)
;;; FDEX
        pop     iy
        ex      (sp),iy
        push    iy
        jp      (iy)
;;; FDFX
        ld      sp,iy

;;; CB0X
        rlc     b
        rlc     c
        rlc     d
        rlc     e
        rlc     h
        rlc     l
        rlc     (hl)
        rlc     a
        rrc     b
        rrc     c
        rrc     d
        rrc     e
        rrc     h
        rrc     l
        rrc     (hl)
        rrc     a
;;; CB1X
        rl      b
        rl      c
        rl      d
        rl      e
        rl      h
        rl      l
        rl      (hl)
        rl      a
        rr      b
        rr      c
        rr      d
        rr      e
        rr      h
        rr      l
        rr      (hl)
        rr      a
;;; CB2X
        sla     b
        sla     c
        sla     d
        sla     e
        sla     h
        sla     l
        sla     (hl)
        sla     a
        sra     b
        sra     c
        sra     d
        sra     e
        sra     h
        sra     l
        sra     (hl)
        sra     a
;;; CB3X
        srl     b
        srl     c
        srl     d
        srl     e
        srl     h
        srl     l
        srl     (hl)
        srl     a
;;; CB4X
        bit     0,b
        bit     0,c
        bit     0,d
        bit     0,e
        bit     0,h
        bit     0,l
        bit     0,(hl)
        bit     0,a
        bit     1,b
        bit     1,c
        bit     1,d
        bit     1,e
        bit     1,h
        bit     1,l
        bit     1,(hl)
        bit     1,a
;;; CB5X
        bit     2,b
        bit     2,c
        bit     2,d
        bit     2,e
        bit     2,h
        bit     2,l
        bit     2,(hl)
        bit     2,a
        bit     3,b
        bit     3,c
        bit     3,d
        bit     3,e
        bit     3,h
        bit     3,l
        bit     3,(hl)
        bit     3,a
;;; CB6X
        bit     4,b
        bit     4,c
        bit     4,d
        bit     4,e
        bit     4,h
        bit     4,l
        bit     4,(hl)
        bit     4,a
        bit     5,b
        bit     5,c
        bit     5,d
        bit     5,e
        bit     5,h
        bit     5,l
        bit     5,(hl)
        bit     5,a
;;; CB7X
        bit     6,b
        bit     6,c
        bit     6,d
        bit     6,e
        bit     6,h
        bit     6,l
        bit     6,(hl)
        bit     6,a
        bit     7,b
        bit     7,c
        bit     7,d
        bit     7,e
        bit     7,h
        bit     7,l
        bit     7,(hl)
        bit     7,a
;;; CB8X
        res     0,b
        res     0,c
        res     0,d
        res     0,e
        res     0,h
        res     0,l
        res     0,(hl)
        res     0,a
        res     1,b
        res     1,c
        res     1,d
        res     1,e
        res     1,h
        res     1,l
        res     1,(hl)
        res     1,a
;;; CB9X
        res     2,b
        res     2,c
        res     2,d
        res     2,e
        res     2,h
        res     2,l
        res     2,(hl)
        res     2,a
        res     3,b
        res     3,c
        res     3,d
        res     3,e
        res     3,h
        res     3,l
        res     3,(hl)
        res     3,a
;;; CBAX
        res     4,b
        res     4,c
        res     4,d
        res     4,e
        res     4,h
        res     4,l
        res     4,(hl)
        res     4,a
        res     5,b
        res     5,c
        res     5,d
        res     5,e
        res     5,h
        res     5,l
        res     5,(hl)
        res     5,a
;;; CBBX
        res     6,b
        res     6,c
        res     6,d
        res     6,e
        res     6,h
        res     6,l
        res     6,(hl)
        res     6,a
        res     7,b
        res     7,c
        res     7,d
        res     7,e
        res     7,h
        res     7,l
        res     7,(hl)
        res     7,a
;;; CBCX
        set     0,b
        set     0,c
        set     0,d
        set     0,e
        set     0,h
        set     0,l
        set     0,(hl)
        set     0,a
        set     1,b
        set     1,c
        set     1,d
        set     1,e
        set     1,h
        set     1,l
        set     1,(hl)
        set     1,a
;;; CBDX
        set     2,b
        set     2,c
        set     2,d
        set     2,e
        set     2,h
        set     2,l
        set     2,(hl)
        set     2,a
        set     3,b
        set     3,c
        set     3,d
        set     3,e
        set     3,h
        set     3,l
        set     3,(hl)
        set     3,a
;;; CBEX
        set     4,b
        set     4,c
        set     4,d
        set     4,e
        set     4,h
        set     4,l
        set     4,(hl)
        set     4,a
        set     5,b
        set     5,c
        set     5,d
        set     5,e
        set     5,h
        set     5,l
        set     5,(hl)
        set     5,a
;;; CBFX
        set     6,b
        set     6,c
        set     6,d
        set     6,e
        set     6,h
        set     6,l
        set     6,(hl)
        set     6,a
        set     7,b
        set     7,c
        set     7,d
        set     7,e
        set     7,h
        set     7,l
        set     7,(hl)
        set     7,a

;;; DDCBXX
        rlc     (ix+05H)
        rrc     (ix+0dH)
        rl      (ix+15H)
        rr      (ix+1dH)
        sla     (ix+25H)
        sra     (ix+2dH)
        srl     (ix+3dH)
        bit     0,(ix+45H)
        bit     1,(ix+4dH)
        bit     2,(ix+55H)
        bit     3,(ix+5dH)
        bit     4,(ix+65H)
        bit     5,(ix+6dH)
        bit     6,(ix+75H)
        bit     7,(ix+7dH)
        res     0,(ix-123)
        res     1,(ix-115)
        res     2,(ix-107)
        res     3,(ix-99)
        res     4,(ix-91)
        res     5,(ix-83)
        res     6,(ix-75)
        res     7,(ix-67)
        set     0,(ix-59)
        set     1,(ix-51)
        set     2,(ix-43)
        set     3,(ix-35)
        set     4,(ix-27)
        set     5,(ix-19)
        set     6,(ix-11)
        set     7,(ix-3)

;;; FDCBXX
        rlc     (iy+05H)
        rrc     (iy+0dH)
        rl      (iy+15H)
        rr      (iy+1dH)
        sla     (iy+25H)
        sra     (iy+2dH)
        srl     (iy+3dH)
        bit     0,(iy+45H)
        bit     1,(iy+4dH)
        bit     2,(iy+55H)
        bit     3,(iy+5dH)
        bit     4,(iy+65H)
        bit     5,(iy+6dH)
        bit     6,(iy+75H)
        bit     7,(iy+7dH)
        res     0,(iy-123)
        res     1,(iy-115)
        res     2,(iy-107)
        res     3,(iy-99)
        res     4,(iy-91)
        res     5,(iy-83)
        res     6,(iy-75)
        res     7,(iy-67)
        set     0,(iy-59)
        set     1,(iy-51)
        set     2,(iy-43)
        set     3,(iy-35)
        set     4,(iy-27)
        set     5,(iy-19)
        set     6,(iy-11)
        set     7,(iy-3)

;;; SUB/AND/XOR/OR/CP instruction can take optionl destination A register.
        org     05A0H

        sub     a,b
        sub     a,c
        sub     a,d
        sub     a,e
        sub     a,h
        sub     a,l
        sub     a,(hl)
        sub     a,a
        and     a,b
        and     a,c
        and     a,d
        and     a,e
        and     a,h
        and     a,l
        and     a,(hl)
        and     a,a
        xor     a,b
        xor     a,c
        xor     a,d
        xor     a,e
        xor     a,h
        xor     a,l
        xor     a,(hl)
        xor     a,a
        or      a,b
        or      a,c
        or      a,d
        or      a,e
        or      a,h
        or      a,l
        or      a,(hl)
        or      a,a
        cp      a,b
        cp      a,c
        cp      a,d
        cp      a,e
        cp      a,h
        cp      a,l
        cp      a,(hl)
        cp      a,a
        sub     a,0d7H
        and     a,0e7H
        xor     a,0efH
        or      a,0f7H
        cp      a,0ffH
        sub     a,(ix-105)
        and     a,(ix-89)
        xor     a,(ix-81)
        or      a,(ix-73)
        cp      a,(ix-65)
        sub     a,(iy-105)
        and     a,(iy-89)
        xor     a,(iy-81)
        or      a,(iy-73)
        cp      a,(iy-65)

        end
        ;; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
