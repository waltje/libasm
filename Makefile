# Copyright 2021 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

help:
	@echo '"make clean"  remove unnecessary files'

clean: 
	$(MAKE) -C cli clean
	$(MAKE) -C test clean
	$(MAKE) -C test/autogen clean
	$(MAKE) -C test/reference clean
	$(MAKE) -C examples clean
	rm -f $$(find . -type f -a -name '*~')

.PHONY: help clean

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
