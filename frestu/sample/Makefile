CC = g++
OPTIONS = -std=c++17 -Wall -I ../.. -pthread -fopenmp
SAMPLES = \
optimization/ga/floyd.out \
optimization/ga/floyd_eigen.out

all: $(SAMPLES)

TARGETS = \
optimization/ga/floyd.cc \
../util/generation/number/random/linear.cc
optimization/ga/floyd.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/floyd_eigen.cc \
../util/generation/number/random/linear.cc
optimization/ga/floyd_eigen.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

.cc.out:
	$(CC) -o $@ $^ $(OPTIONS)

.h.out:
	$(CC) -o $@ $^ $(OPTIONS)

.PHONY: clean
clean:
	find . -name '*.out' | xargs rm

.PHONY: exec
exec:
	find . -name '*.out' | bash