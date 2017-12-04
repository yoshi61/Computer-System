# must specify make target
all: assembler-m assembler-f

# C++ 11
CXXFLAGS=--std=c++11 -I. -Iincludes

clean:
	rm -rf assembler-m assembler-f *.o

AFCPP:=assembler-f.cpp ${wildcard assembler-f-*.cpp}
AMCPP:=assembler-m.cpp ${wildcard assembler-m-*.cpp}
MYCPP:=${wildcard shared-*.cpp}
AFO:=${AFCPP:.cpp=.o}
AMO:=${AMCPP:.cpp=.o}
MYO:=${MYCPP:.cpp=.o}

assembler-m: ${AMO} ${MYO} lib/main-assembler-m.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

assembler-f: ${AFO} ${MYO} lib/main-assembler-f.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

test: test-demo test-assembler-m test-assembler-f

test-demo: assembler-m assembler-f
	-@bash tests/run-tests demo-assembler-m
	-@bash tests/run-tests demo-assembler-f

test-assembler-m: assembler-m
	-@bash tests/run-tests assembler-m

test-assembler-f: assembler-f
	-@bash tests/run-tests assembler-f

