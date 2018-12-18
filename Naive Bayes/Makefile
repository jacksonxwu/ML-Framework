OS := $(shell uname)

ifeq ($(OS), Darwin)
    CPPC := g++ -g -pedantic -Wall -Wextra
    CC := gcc -g -pedantic  -Wall -Wextra
else
    ifeq ($(OS), Linux)
    	CC := gccx
    endif
endif

all: bayes predict gendata

bayes: bayes.cpp 
	$(CPPC) -lm -o bayes bayes.cpp

naivebayes: naivebayes.cpp csvparser.c csvparser.h gendata.cpp gendata.h bayes.cpp bayes.h predict.h predict.cpp
	$(CPPC) naivebayes.cpp csvparser.c bayes.cpp predict.cpp gendata.cpp -lm -o naivebayes

predict: predict.cpp
	$(CPPC) predict.cpp -lm -o predict

gendata: gendata.cpp csvparser.c bayes.cpp bayes.h predict.h predict.cpp
	$(CPPC) gendata.cpp csvparser.c bayes.cpp predict.cpp -lm -o gendata

clean:
	rm -rf *.o bayes predict gendata naivebayes *.dSYM
