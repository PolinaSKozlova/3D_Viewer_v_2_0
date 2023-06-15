#all, install, uninstall, clean, dvi, dist, tests, gcov_report
CC = g++
CC_FLAGS = -Wall -Wextra -Werror -std=c++17 #-pedantic-errors
TEST_FLAGS = -lgtest -pthread
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
SANITIZER_FLAGS = #-fsanitize=address
PROJECT_NAME = s21_3DViewer_v_2_0
SRCS=$(wildcard s21*.cc,*/s21*.cc)
OBJ_FILES=$(patsubst %.cc,%.o,$(SRCS))
REPORT_DIR = report

run: clean
	clear
	$(CC) $(CC_FLAGS) $(SANITIZER_FLAGS) affine_transformations/s21_matrix4X4.cc affine_transformations/s21_matrix_transform.cc parser/s21_parser.cc $(OBJ_FILES) run.cc -o run -g
#	./run

all: install

install:
	cmake -Wno-deprecated -S . -B build -G "Unix Makefiles"
	make -C build
	cd ./build && rm -rf CMakeFiles 3DViewer_2_autogen cmake_install.cmake CMakeCache.txt Makefile qrc*
	open build

uninstall:
	rm -rf build

clean:
	rm -rf *.o *.gcno *.gcda *.info *.a $(REPORT_DIR) v_tests run *.dSYM 3DViewer_v_2_0.tar.gz CMakeLists.txt.user

dvi:
	makeinfo -o report --html --no-warn --no-validate --force doc.texi
	open report/index.html

dist:
	tar -czf 3DViewer_v_2_0.tar.gz Makefile doc.texi CMakeLists.txt
	open .

tests: 
	clear
	$(CC) $(CC_FLAGS) viewer_test/*.cc affine_transformations/s21_*.cc -o v_tests $(TEST_FLAGS)
	./v_tests

gcov_report:
	clear
	$(CC) $(CC_FLAGS) $(COVERAGE_FLAGS) viewer_test/*.cc affine_transformations/s21_*.cc -o v_tests $(TEST_FLAGS)
	./v_tests
	lcov -t "$@" -o $@.info -c -d . --no-external
	lcov -r $@.info "*include*" -o $@.info
	genhtml -o ./$(REPORT_DIR) $@.info
	open ./report/index.html

%.o:%.cc
	$(CC) -c $(FLAGS) $< -o $@ -I .

valgrind: tests
	valgrind --leak-check=full \
        	 --show-leak-kinds=all \
         	 --track-origins=yes \
         	 --verbose \
			 ./c_tests

.PHONY:
	all, clean, tests, gcov_report, install, uninstall, dvi, dist

clang:
	clang-format -i --style=Google **/s21*.cc **/s21*.h 
	clang-format -n --style=Google **/s21*.cc **/s21*.h

cppcheck:
	cppcheck --enable=all --language=c++ --suppress=missingIncludeSystem **/*s21*.cc **/*s21*.h

push:
	git push origin matrix4x4


