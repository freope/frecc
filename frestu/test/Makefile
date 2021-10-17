CC = g++
OPTIONS = -std=c++17 -Wall -I ../.. -pthread -fopenmp -lgtest_main -lgtest
SUITES = \
optimization/ga/crossover.out \
optimization/ga/individual.out \
optimization/ga/population.out \
optimization/ga/selection.out \
optimization/ga/gene/gene_discrete.out \
optimization/ga/gene/gene_continuous.out \
optimization/ga/gene/gene_continuous_logscale.out \
util/generation/number/random/linear.out \
util/generation/number/random/logscale.out \
util/generation/number/sequential/linear.out \
util/generation/number/sequential/logscale.out \
util/iteration/tuple/application.out \
util/sampling.out

all: $(SUITES)

TARGETS = \
optimization/ga/crossover.cc \
../optimization/ga/crossover.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc
optimization/ga/crossover.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGET = \
optimization/ga/individual.cc \
../optimization/ga/individual.h \
../optimization/ga/gene/gene_discrete.h \
../optimization/ga/gene/gene_continuous.h \
../optimization/ga/gene/gene_continuous_logscale.h \
../optimization/ga/crossover.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc \
../util/generation/number/sequential/linear.h \
../util/generation/number/random/logscale.h \
../util/generation/number/random/logscale.cc \
../util/generation/number/sequential/logscale.h
optimization/ga/individual.out: $(TARGET)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/population.cc \
../optimization/ga/population.h \
../optimization/ga/individual.h \
../optimization/ga/gene/gene_discrete.h \
../optimization/ga/gene/gene_continuous.h \
../optimization/ga/gene/gene_continuous_logscale.h \
../optimization/ga/crossover.h \
../optimization/ga/selection.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc \
../util/generation/number/sequential/linear.h \
../util/generation/number/random/logscale.h \
../util/generation/number/random/logscale.cc \
../util/generation/number/sequential/logscale.h
optimization/ga/population.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/selection.cc \
../optimization/ga/selection.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc
optimization/ga/selection.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_discrete.cc \
../optimization/ga/gene/gene_discrete.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc \
../util/generation/number/sequential/linear.h
optimization/ga/gene/gene_discrete.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_continuous.cc \
../optimization/ga/gene/gene_continuous.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc \
../util/generation/number/sequential/linear.h
optimization/ga/gene/gene_continuous.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_continuous_logscale.cc \
../optimization/ga/gene/gene_continuous_logscale.h \
../optimization/ga/gene/gene_continuous.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc \
../util/generation/number/sequential/linear.h \
../util/generation/number/random/logscale.h \
../util/generation/number/random/logscale.cc \
../util/generation/number/sequential/logscale.h
optimization/ga/gene/gene_continuous_logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/random/linear.cc \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc
util/generation/number/random/linear.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/random/logscale.cc \
../util/generation/number/random/logscale.h \
../util/generation/number/random/logscale.cc
util/generation/number/random/logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/sequential/linear.cc \
../util/generation/number/sequential/linear.h
util/generation/number/sequential/linear.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/sequential/logscale.cc \
../util/generation/number/sequential/logscale.h
util/generation/number/sequential/logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/iteration/tuple/application.cc \
../util/iteration/tuple/application.h
util/iteration/tuple/application.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/sampling.cc \
../util/sampling.h \
../util/generation/number/random/linear.h \
../util/generation/number/random/linear.cc
util/sampling.out: $(TARGETS)
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