CC = g++
OPTIONS = -std=c++17 -Wall -I ../.. -pthread -fopenmp -lgtest_main -lgtest
SUITES = \
agent/trader/position.out \
data_type/data_frame/data_frame.out \
feature_extraction/time_series/extractor_ema.out \
feature_extraction/time_series/extractor_sma.out \
feature_extraction/time_series/extractor_sstd.out \
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
util/io/table_reader.out \
util/iteration/tuple/application.out \
util/sampling.out \
practice.out

all: $(SUITES)

TARGETS = \
agent/trader/position.cc
agent/trader/position.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
feature_extraction/time_series/extractor_ema.cc \
../util/io/table_reader.cc
feature_extraction/time_series/extractor_ema.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
feature_extraction/time_series/extractor_sma.cc \
../util/io/table_reader.cc
feature_extraction/time_series/extractor_sma.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
feature_extraction/time_series/extractor_sstd.cc \
../util/io/table_reader.cc
feature_extraction/time_series/extractor_sstd.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
data_type/data_frame/data_frame.cc \
../util/io/table_reader.cc
data_type/data_frame/data_frame.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/crossover.cc \
../util/generation/number/random/linear.cc
optimization/ga/crossover.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGET = \
optimization/ga/individual.cc \
../util/generation/number/random/linear.cc \
../util/generation/number/random/logscale.cc
optimization/ga/individual.out: $(TARGET)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/population.cc \
../util/generation/number/random/linear.cc \
../util/generation/number/random/logscale.cc
optimization/ga/population.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/selection.cc \
../util/generation/number/random/linear.cc
optimization/ga/selection.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_discrete.cc \
../util/generation/number/random/linear.cc
optimization/ga/gene/gene_discrete.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_continuous.cc \
../util/generation/number/random/linear.cc
optimization/ga/gene/gene_continuous.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
optimization/ga/gene/gene_continuous_logscale.cc \
../util/generation/number/random/linear.cc \
../util/generation/number/random/logscale.cc
optimization/ga/gene/gene_continuous_logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/random/linear.cc \
../util/generation/number/random/linear.cc
util/generation/number/random/linear.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/random/logscale.cc \
../util/generation/number/random/logscale.cc
util/generation/number/random/logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/sequential/linear.cc
util/generation/number/sequential/linear.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/generation/number/sequential/logscale.cc
util/generation/number/sequential/logscale.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/io/table_reader.cc \
../util/io/table_reader.cc
util/io/table_reader.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/iteration/tuple/application.cc
util/iteration/tuple/application.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
util/sampling.cc \
../util/generation/number/random/linear.cc
util/sampling.out: $(TARGETS)
	$(CC) -o $@ $^ $(OPTIONS)

TARGETS = \
practice.cc \
../util/io/table_reader.cc \
../data_type/data_frame/data_frame.h
practice.out: $(TARGETS)
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