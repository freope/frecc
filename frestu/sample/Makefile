CC = g++
OPTIONS = -std=c++17 -Wall -I ../.. -pthread -fopenmp
SAMPLES = \
optimization/ga/floyd.out

all: $(SAMPLES)

TARGETS = \
optimization/ga/floyd.cc \
../optimization/ga/population.h \
../optimization/ga/individual.h \
../optimization/ga/gene/gene_discrete.h \
../optimization/ga/crossover.h \
../optimization/ga/selection.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc
optimization/ga/floyd.out: $(TARGETS)
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