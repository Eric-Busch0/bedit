CXX=g++
OUT=bedit
SRCS=main.cpp
LD=ncurses
defualt:
	$(CXX) $(SRCS) -l${LD} -o $(OUT) --std=c++17
clean:
	rm $(OUT)
run:
	 ./$(OUT)
