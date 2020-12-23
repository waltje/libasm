; Copyright 2020 Tadashi G. Takaoka
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

        cpu     z8002
        org     1000h
data1234: equ   1234h
data5678: equ   5678h
        include "test_z8002.inc"
        include "test_z8000_io.inc"
        end

; Local Variables:
; mode: asm
; End:
; vim: set ft=asm:
