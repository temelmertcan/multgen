



multgen.o : adders.o wallace.o pp_gen.o 
	g++ src/multgen.cpp build/adders.o build/pp_gen.o build/wallace.o -o multgen
	@echo "Executable multgen is created."

pp_gen.o : src/pp_gen.cpp src/pp_gen.h
	g++ src/pp_gen.cpp -c -o build/pp_gen.o


adders.o :  src/adders.cpp src/adders.h
	mkdir -p build
	g++ src/adders.cpp -c -o build/adders.o


wallace.o :  src/wallace.cpp src/wallace.h adders.o pp_gen.o
	g++ src/wallace.cpp -c -o build/wallace.o


