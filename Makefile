CXX=clang++ 

EXEC=lesh
OBJS=${EXEC}.o value.o env.o parse.o primitive.o

${EXEC}:${OBJS}
	${CXX} -o ${EXEC} ${OBJS}
clean:
	rm ${EXEC} ${OBJS}

CPPFLAGS=-ggdb -Wall -std=c++11

lesh.o:lesh.cpp lesh.hpp
value.o:value.cpp lesh.hpp
env.o:env.cpp lesh.hpp
parse.o:parse.cpp lesh.hpp
primitive.o:primitive.cpp lesh.hpp
