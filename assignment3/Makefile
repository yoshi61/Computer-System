# all the executables
EXECS= parser pretty codegen opt-r
JEXECS=$(patsubst %,j%,${EXECS})
TESTS=$(patsubst %,test-%,${EXECS})
MYALL:=${wildcard shared-*.cpp}
MYPARSER:=parser.cpp ${wildcard parser-*.cpp}
MYCODEGEN:=codegen.cpp ${wildcard codegen-*.cpp}
MYPRETTY:=pretty.cpp ${wildcard pretty-*.cpp}
MYOPTIMISER_R:=optimiser-r.cpp ${wildcard optimiser-r-*.cpp}

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

all: ${EXECS}

test: ${TESTS}

help:
	-@echo make options are one of:
	-@echo " all, parser, codegen, pretty, opt-r, opt-e or compiler"
	-@echo testing options are one of:
	-@echo " test-all, test-parser, test-codegen, test-pretty, test-opt-r, test-opt-e or test-compiler"
	-@echo


new-tests:
	-@(bash tests/make-tests)

clean:
	-rm -rf ${JEXECS} *.o

%.o: %.cpp %.h
	g++ ${CXXFLAGS} -c $<

.PHONY: ${EXECS}
parser: jparser
test-parser-dnc: parser
	-@(bash tests/run-tests parser-dnc)
test-parser: parser
	-@(bash tests/run-tests parser-dnc)
	-@(bash tests/run-tests parser)
jparser: ${MYPARSER} ${MYALL} lib/main-parser.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

pretty: jpretty
test-pretty: pretty
	-@(bash tests/run-tests pretty)
jpretty: ${MYPRETTY} ${MYALL} lib/main-pretty.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

codegen: jcodegen
test-codegen: codegen
	-@(bash tests/run-tests codegen)
jcodegen: ${MYCODEGEN} ${MYALL} lib/main-codegen.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

opt-r: jopt-r
test-opt-r: opt-r
	-@(bash tests/run-tests opt-r)
jopt-r: ${MYOPTIMISER_R} ${MYALL} lib/main-optimiser-r.o lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

# to run tests with working versions of the programs
demo-test: demo-parser demo-pretty demo-codegen demo-opt-r
demo-parser-dnc:
	-@(bash tests/run-tests demo-parser-dnc)
demo-parser:
	-@(bash tests/run-tests demo-parser-dnc)
	-@(bash tests/run-tests demo-parser)
demo-pretty:
	-@(bash tests/run-tests demo-pretty)
demo-codegen:
	-@(bash tests/run-tests demo-codegen)
demo-opt-r:
	-@(bash tests/run-tests demo-opt-r)
