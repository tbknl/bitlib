
SUITES_DIR = ./suites
LIB_DIR = ../include
TEST_SOURCES = ./main.cpp $(wildcard ${SUITES_DIR}/*.cpp)
TEST_HEADERS = $(wildcard ./*.h)
LIB_SOURCES = $(wildcard ${LIB_DIR}/*.h)

LINK_LIBS = UnitTest++


.PHONY: test

test: main
	./main

main: $(TEST_SOURCES) $(TEST_HEADERS) $(LIB_SOURCES)
	g++ $(TEST_SOURCES) -o $@ -Wall -O2 -I${LIB_DIR} $(addprefix -l, ${LINK_LIBS})

