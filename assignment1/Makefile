# must specify make target
all: tokeniser-m tokeniser-f

# C++ 11
CXXFLAGS=--std=c++11 -I. -Iincludes

MYCPP:=${wildcard my*.cpp}
MYO:=${MYCPP:.cpp=.o}

tokeniser-m: lib/mainm.o tokensm.o ${MYO} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

tokeniser-f: lib/mainf.o tokensf.o ${MYO} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

clean:
	rm -f tokeniser-m tokeniser-f *.o ${MYO}

test: test-tokeniser-m test-tokeniser-f

test-tokeniser-m: tokeniser-m
	-@bash testsm/run-tests tokeniser-m

test-tokeniser-f: tokeniser-f
	-@bash testsf/run-tests tokeniser-f
