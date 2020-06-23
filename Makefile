#   - - - - - - - - - - - - -    configuration     - - - - - - - - - - - - -   #

BIN = treee
PREFIX = /usr/local

LIBS = -lncurses

EXTDIR = ext
INCDIR = include
SRCDIR = src

BINDIR = .bin
OBJDIR = .obj
DEPDIR = .dep

SRC = disp.c \
      file.c \
      main.c

CFLAGS  = -std=gnu99     \
					-g             \
					-pedantic      \
					-Wall          \
					-I/usr/include \
					-I${INCDIR}    \
					-I${EXTDIR}
LDFLAGS = ${LIBS}

#   - - - - - - - - - - - - - - -    phony   - - - - - - - - - - - - - - - -   #

all: ${BINDIR}/${BIN}

run: ${BINDIR}/${BIN}
	./${BINDIR}/${BIN}

install: ${BINDIR}/${BIN}
	cp $< ${PREFIX}/bin

uninstall:
	rm -f ${PREFIX}/bin/${BIN}

clean:
	rm -rf ./${BINDIR} ./${OBJDIR} ./${DEPDIR}

.PHONY: all run clean

#   - - - - - - - - - - - - - - -   objects    - - - - - - - - - - - - - - -   #

vpath %.c ${SRCDIR}

OBJ = $(patsubst %.c,${OBJDIR}/%.o,$(SRC))

${OBJ}: | ${OBJDIR}

${OBJDIR}:
	@mkdir -p $@

${OBJDIR}/%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

#   - - - - - - - - - - - - - - -    binary    - - - - - - - - - - - - - - -   #

${BINDIR}:
	@mkdir -p $@

${BINDIR}/${BIN}: ${BINDIR} ${OBJ} ${INC}
	${CC} -o $@ ${OBJ} ${LDFLAGS}
	@chmod 755 $@

#   - - - - - - - - - - - - - -   dependencies   - - - - - - - - - - - - - -   #

$(patsubst %.c,${DEPDIR}/%.d,$(SRC)): | ${DEPDIR}

${DEPDIR}:
	@mkdir -p $@

${DEPDIR}/%.d: %.c
	@${CC} ${CFLAGS}                          \
    -MF"$@" -MG -M -MP                      \
    -MT"$@"                                 \
    -MT"$(patsubst %.c,${DEPDIR}/%.o,${<})" \
    "$<"

include $(patsubst %.c,${DEPDIR}/%.d,$(SRC))
