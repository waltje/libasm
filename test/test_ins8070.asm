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

        cpu     8070
        org     0x0100

;;; 0X
        nop
        xch     a,e
        ld      a,s
        ld      s,a
        push    ea
        ld      t,ea
        push    a
        ld      ea,t
        sr      ea
        div     ea,t
        sl      a
        sl      ea
;;; 1X
        call    0
        call    1
        call    2
        call    3
        call    4
        call    5
        call    6
        call    7
        call    8
        call    9
        call    10
        call    11
        call    12
        call    13
        call    14
        call    15
;;; 2X
        jsr     0x2222
        pli     p2,#0x2423
        pli     p3,#0x2524
        jmp     0x2626
        ld      sp,#0x2726
        ld      p2,#0x2827
        ld      p3,#0x2928
        mpy     ea,t
        bnd     0x0162
        ssm     p2
        ssm     p3
;;; 3X
        ld      ea,pc
        ld      ea,sp
        ld      ea,p2
        ld      ea,p3
        pop     a
        and     s,#0x3a
        pop     ea
        or      s,#0x3c
        sr      a
        srl     a
        rr      a
        rrl     a
;;; 4X
        ld      a,e
        ld      pc,ea
        ld      sp,ea
        ld      p2,ea
        ld      p3,ea
        ld      e,a
        xch     ea,pc
        xch     ea,sp
        xch     ea,p2
        xch     ea,p3
;;; 5X
        and     a,e
        push    pc
        push    p2
        push    p3
        or      a,e
        ret
        pop     p2
        pop     p3
;;; 6X
        xor     a,e
        bp      0x01BE
        bp      103,p2
        bp      104,p3
        bz      0x01CC
        bz      111,p2
        bz      112,p3
;;; 7X
        add     a,e
        bra     0x01DB
        bra     117,p2
        bra     118,p3
        sub     a,e
        bnz     0x01EA
        bnz     127,p2
        bnz     -128,p3
;;; 8X
        ld      ea,0x00F3,pc
        ld      ea,-126,sp
        ld      ea,-125,p2
        ld      ea,-124,p3
        ld      ea,#0x8685
        ld      ea,0xFF86
        ld      ea,@-121,p2
        ld      ea,@-120,p3
        st      ea,0x010C,pc
        st      ea,-118,sp
        st      ea,-117,p2
        st      ea,-116,p3
        st      ea,0xFF8E
        st      ea,@-113,p2
        st      ea,@-112,p3
;;; 9X
        ild     a,0x0122,pc
        ild     a,-110,sp
        ild     a,-109,p2
        ild     a,-108,p3
        ild     a,0xFF96
        ild     a,@-105,p2
        ild     a,@-104,p3
        dld     a,0x0138,pc
        dld     a,-102,sp
        dld     a,-101,p2
        dld     a,-100,p3
        dld     a,0xFF9E
        dld     a,@-97,p2
        dld     a,@-96,p3
;;; AX
        ld      t,0x014E,pc
        ld      t,-94,sp
        ld      t,-93,p2
        ld      t,-92,p3
        ld      t,#0xA6A5
        ld      t,0xFFA6
        ld      t,@-89,p2
        ld      t,@-88,p3
;;; BX
        add     ea,0x016F,pc
        add     ea,-78,sp
        add     ea,-77,p2
        add     ea,-76,p3
        add     ea,#0xB6B5
        add     ea,0xFFB6
        add     ea,@-73,p2
        add     ea,@-72,p3
        sub     ea,0x0188,pc
        sub     ea,-70,sp
        sub     ea,-69,p2
        sub     ea,-68,p3
        sub     ea,#0xBEBD
        sub     ea,0xFFBE
        sub     ea,@-65,p2
        sub     ea,@-64,p3
;;; CX
        ld      a,0x01A1,pc
        ld      a,-62,sp
        ld      a,-61,p2
        ld      a,-60,p3
        ld      a,#0xC5
        ld      a,0xFFC6
        ld      a,@-57,p2
        ld      a,@-56,p3
        st      a,0x01B9,pc
        st      a,-54,sp
        st      a,-53,p2
        st      a,-52,p3
        st      a,0xFFCE
        st      a,@-49,p2
        st      a,@-48,p3
;;; DX
        and     a,0x01CF,pc
        and     a,-46,sp
        and     a,-45,p2
        and     a,-44,p3
        and     a,#0xD5
        and     a,0xFFD6
        and     a,@-41,p2
        and     a,@-40,p3
        or      a,0x01E7,pc
        or      a,-38,sp
        or      a,-37,p2
        or      a,-36,p3
        or      a,#0xDD
        or      a,0xFFDE
        or      a,@-33,p2
        or      a,@-32,p3
;;; EX
        xor     a,0x01FF,pc
        xor     a,-30,sp
        xor     a,-29,p2
        xor     a,-28,p3
        xor     a,#0xE5
        xor     a,0xFFE6
        xor     a,@-25,p2
        xor     a,@-24,p3
;;; FX
        add     a,0x021F,pc
        add     a,-14,sp
        add     a,-13,p2
        add     a,-12,p3
        add     a,#0xF5
        add     a,0xFFF6
        add     a,@-9,p2
        add     a,@-8,p3
        sub     a,0x0237,pc
        sub     a,-6,sp
        sub     a,-5,p2
        sub     a,-4,p3
        sub     a,#0xFD
        sub     a,0xFFFE
        sub     a,@-1,p2
        sub     a,@0,p3

        end
        ;; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
