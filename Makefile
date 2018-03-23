.PHONY: all clean

FLAGS:=-std=c++11 -Wall -Wextra -pedantic -march=native -O3 $(CXXFLAGS)
FLAGS:=$(FLAGS) -Iinclude

ALL=compare verify benchmark statistics

OBJ=obj/input_generator.o \
    obj/block_info.o

DEPS=include/scalar-parser.h \
     include/sse-convert.h \
     include/sse-matcher.h \
     include/sse-parser.h

all: $(ALL)

compare: test/compare.cpp $(DEPS) $(OBJ)
	$(CXX) $(FLAGS) $< $(OBJ) -o $@

verify: test/verify.cpp $(DEPS) $(OBJ)
	$(CXX) $(FLAGS) $< $(OBJ) -o $@

STATISTICS_OBJ=$(OBJ) obj/sse-parser-statistics.o
statistics: test/statistics.cpp $(DEPS) $(STATISTICS_OBJ)
	$(CXX) $(FLAGS) $< $(STATISTICS_OBJ) -o $@

benchmark: test/benchmark.cpp $(DEPS) include/time_utils.h $(OBJ)
	$(CXX) $(FLAGS) $< $(OBJ) -o $@

obj/input_generator.o: test/input_generator.cpp include/input_generator.h
	$(CXX) $(FLAGS) $< -c -o $@

obj/block_info.o: src/block_info.cpp src/block_info.inl include/block_info.h
	$(CXX) $(FLAGS) $< -c -o $@

obj/sse-parser-statistics.o: src/sse-parser-statistics.cpp
	$(CXX) $(FLAGS) $< -c -o $@

src/block_info.inl: scripts/generator.py scripts/writer.py
	python $< $@

clean:
	$(RM) $(ALL) obj/*.o
