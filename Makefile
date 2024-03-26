CFLAGS = -O3 -std=gnu++20 -Wall -Wextra -Wshadow -D_GLIBCXX_ASSERTIONS -fmax-errors=2 -DLOCAL

print.o : print.cpp
	g++ print.h ${CFLAGS}
	g++ -c print.cpp -o print.o ${CFLAGS}
clean:
	rm -f print.o print.h.gch
