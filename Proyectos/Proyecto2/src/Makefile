textura: LoadModels.cpp getbmp.o
	g++ -g -o textura LoadModels.cpp getbmp.o -framework OpenGL -lglut -lglew -lassimp -lIL -arch x86_64

getbmp.o: getbmp.cpp getbmp.h
	g++ -c getbmp.cpp getbmp.h

clean:
	rm -f getbmp.o
	rm -r textura.dSYM
	rm -f textura
	rm -f getbmp.h.gch
