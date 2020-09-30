# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#    _
#   | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
#   | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020 Justin Collier
#   | |_| | |  __/  __/  __/
#    \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the internalied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

MAKEFLAGS += --no-print-directory

all: build/treee

build:
	mkdir -p $@
	cmake -B$@

build/treee: build $(shell find src -name *.cc) $(shell find src -name *.h)
	cmake --build $< --target treee

run: build/treee
	./build/treee .

install: build
	cmake --install $<

uninstall:
	${RM} $(shell cat build/install_manifest.txt)

clean:
	${RM} -r build

.PHONY: all run install uninstall clean
