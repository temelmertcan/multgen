



multgen.o : adders.o wallace.o pp_gen.o dadda.o
	g++ -std=c++11  src/multgen.cpp build/adders.o build/pp_gen.o build/dadda.o build/wallace.o -o multgen
	@echo "Executable multgen is created."

pp_gen.o : src/pp_gen.cpp src/pp_gen.h
	g++ -std=c++11 src/pp_gen.cpp -c -o build/pp_gen.o


adders.o :  src/adders.cpp src/adders.h
	mkdir -p build
	g++ src/adders.cpp -c -o build/adders.o


wallace.o :  src/wallace.cpp src/wallace.h adders.o pp_gen.o
	g++ src/wallace.cpp -c -o build/wallace.o


# tree.o : src/wallace.cpp src/wallace.h src/dadda.cpp src/dadda.h adders.o pp_gen.o
# 	g++ src/dadda.cpp src/wallace.cpp -c -o build/tree.o


dadda.o :  src/dadda.cpp src/dadda.h adders.o pp_gen.o wallace.o
	g++ src/dadda.cpp -c -o build/dadda.o


