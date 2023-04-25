



multgen.o : adders.o wallace.o pp_gen.o dadda.o
	g++ -std=c++11  src/multgen.cpp build/adders.o build/pp_gen.o build/dadda.o build/wallace.o -o multgen
	@echo "Executable multgen is created."

wallace.o :  src/wallace.cpp src/wallace.h adders.o pp_gen.o
	g++ -std=c++11 src/wallace.cpp -c -o build/wallace.o

dadda.o :  src/dadda.cpp src/dadda.h adders.o pp_gen.o wallace.o
	g++ -std=c++11 src/dadda.cpp -c -o build/dadda.o

pp_gen.o : src/pp_gen.cpp src/pp_gen.h
	g++ -std=c++11 src/pp_gen.cpp -c -o build/pp_gen.o

adders.o :  src/adders.cpp src/adders.h
	mkdir -p build
	g++ src/adders.cpp -c -o build/adders.o

examples : multgen.o
	@rm -rf examples/*.sv examples/*~; mkdir -p examples; cd examples;\
	../multgen -type StandAlone -tree DT -pp UB16 -adder KS -in1size 16 -in2size 16;\
	../multgen -type StandAlone -tree DT -pp SB4 -adder HC -in1size 8 -in2size 8 -outsize 12;\
	../multgen -type FourMult -tree c42 -pp SB4 -adder JSkCond -in1size 16 -outsize 16;\
	../multgen -type MAC -tree WT -pp SB8 -adder LF -in1size 8 -in2size 10 -in3size 6;\
	../multgen -type DOT -tree DT -pp UB4 -adder LF -in1size 8 -in2size 8 -dotsize 4 -in3size 10 -outsize 10;\
	../multgen -type FourMult -tree DT -pp SSP -adder RP -in1size 16 -in2size 16;\
	../multgen -type FourMult -tree DT -pp SB16 -adder JSkCond -in1size 12 -in2size 12 -outsize 16 -shift 8;\
	echo	 "Examples are created in examples/."
