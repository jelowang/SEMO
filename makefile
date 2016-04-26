VPATH = \
	common: \
	front-para/c-front-para: \
	front-para/lac-front-para: \
	mid-para/mid-para: \
	back-para/arm-back-para

OBJECTS = \
	common/evalor.o \
	common/mopo.o \
	common/sccl.o \
	common/schal.o \
	front-para/c-front-para/c-errors.o \
	front-para/c-front-para/c-lexer.o \
	front-para/c-front-para/c-parser.o \
	front-para/c-front-para/c-presor.o \
	front-para/lac-front-para/lac-lexer.o \
	mid-para/elf-gen.o \
	mid-para/lac.o \
	mid-para/lac-gentor.o \
	mid-para/lgnosia.o \
	mid-para/regoc.o \
	mid-para/symbole.o \
	back-para/arm-back-para/arm-asm-lexer.o \
	back-para/arm-back-para/arm-asmmap.o \
	back-para/arm-back-para/arm-asmor.o \
	back-para/arm-back-para/arm-assemer.o \
	sc.o \
	main.o

evalor.o :
mopo.o :
sccl.o :
schal.o :

c-errors.o
c-lexer.o
c-parser.o
c-presor.o

lac-lexer.o

elf-gen.o
lac.o
lac-gentor.o
lgnosia.o
regoc.o
symbole.o

arm-asm-lexer.o
arm-asmmap.o
arm-asmor.o
arm-assemer.o

semo : $(OBJECTS)
gcc -o semo $(OBJECTS)

.PHONY : clean
clean :
-rm edit $(OBJECTS)