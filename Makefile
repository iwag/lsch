CXX=g++

EXEC=lesh
OBJS=${EXEC}.o value.o env.o parse.o

${EXEC}:${OBJS}
	${CXX} -o ${EXEC} ${OBJS}
clean:
	rm ${EXEC} ${OBJS}

CPPFLAGS=-ggdb -Wall

lesh.o:lesh.cpp lesh.hpp
value.o:value.cpp lesh.hpp
env.o:env.cpp lesh.hpp
parse.o:parse.cpp lesh.hpp
