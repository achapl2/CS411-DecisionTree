Assignment11: Parser.o Node.o Assignment11.o
	g++ -std=c++14 Parser.o Node.o Assignment11.o -o a

Parser.o: Parser.cpp Parser.h
	g++ -std=c++14 -c -g Parser.cpp

Node.o: Node.cpp Node.h
	g++ -std=c++14 -c -g Node.cpp

Assignment11.o: Assignment11.cpp
	g++ -std=c++14 -c -g Assignment11.cpp

clean:
	rm *o a.exe