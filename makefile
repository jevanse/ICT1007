TEST_SOURCE=test_related/algorithm_testing.c
TEST_HEADER=test_related/algorithm_testing.h

all: solutions/paper_4

solutions/paper_4: paper_4/edrr.c $(TEST_SOURCE) $(TEST_HEADER)
	mkdir -p solutions
	gcc paper_4/edrr.c $(TEST_SOURCE) $(TEST_HEADER) -o solutions/paper_4

solutions/paper_4_debug: paper_4/edrr.c $(TEST_SOURCE) $(TEST_HEADER)
	mkdir -p solutions
	gcc -g paper_4/edrr.c $(TEST_SOURCE) $(TEST_HEADER) -o solutions/paper_4

clean:
	rm -rf solutions/