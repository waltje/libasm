;;; Copyright 2021 Tadashi G. Takaoka
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

        cpu     msm80c48
        org     0100h
        include "test_i8039.inc"
        org     0300h
        include "test_i8048.inc"
        org     0380h
        include "test_i80c39.inc"
        org     0400h
        include "test_msm80c39.inc"
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
