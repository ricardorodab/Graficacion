g++ $1 -g getbmp.o -framework OpenGL -lglut -lglew -lassimp -lIL -arch x86_64
mv ./a.out ./$2
