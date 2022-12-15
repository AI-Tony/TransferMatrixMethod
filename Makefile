BIN=
SRC=src/
LIB=src/

TMM: main.o MTM.o TransferMatrixMethod.o Parser.o interface.o
	g++ -Wall -o $(BIN)TMM $(SRC)main.o $(SRC)MTM.o $(SRC)TransferMatrixMethod.o $(SRC)Parser.o $(SRC)interface.o

main.o: $(SRC)main.cpp 
	g++ -o $(SRC)main.o -c $(SRC)main.cpp -I$(LIB)

MTM.o: $(SRC)MTM.cpp
	g++ -o $(SRC)MTM.o -c $(SRC)MTM.cpp -I$(LIB)

TransferMatrixMethod.o: $(SRC)TransferMatrixMethod.cpp
	g++ -o $(SRC)TransferMatrixMethod.o -c $(SRC)TransferMatrixMethod.cpp -I$(LIB)

Parser.o: $(SRC)Parser.cpp
	g++ -o $(SRC)Parser.o -c $(SRC)Parser.cpp -I$(LIB)

interface.o: $(SRC)interface.cpp
	g++ -o $(SRC)interface.o -c $(SRC)interface.cpp -I$(LIB)

clean: 
	rm -f $(BIN)main
	rm -f $(SRC)*.o