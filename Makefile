CPFLAGS = -Wall -O2 -std=c++11

MapEmbed: main.cpp BOBHASH32.h trace.h
	g++ -o RangeBF main.cpp $(CPFLAGS)

clean: 
	rm RangeBF
