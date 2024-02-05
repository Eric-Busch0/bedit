CXX=g++
OUT=bedit
SRCS=main.cpp
LD=ncurses

SRCS = $(wildcard *.cpp)
defualt:
	$(CXX) $(SRCS) -l${LD} -o build/$(OUT) --std=c++17
clean:
	rm build/$(OUT)
run:
	 ./build/$(OUT)
