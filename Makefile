.PHONY: all clean debug_tests tags test test_leaks install install_latest_release

BINARY = "mdtoc"

TEST_BINARY = "check_mdtoc"

COMPILE_DATE = "$(shell date "+%Y-%m-%d")"

GIT_TAG = "$(shell git describe --tags --abbrev=0)"

CFLAGS = -g -std=gnu23 -Wall \
		 -DVERSION=\"$(GIT_TAG)\" \
		 -DCOMPILE_DATE=\"$(COMPILE_DATE)\"

SOURCE_DIR = "src"

SOURCE_FILES = \
			   $(SOURCE_DIR)/chapter.c \
			   $(SOURCE_DIR)/interactive.c \
			   $(SOURCE_DIR)/main.c \
			   $(SOURCE_DIR)/parser.c \
			   $(SOURCE_DIR)/pretty_printer.c \
			   $(SOURCE_DIR)/stream.c \
			   $(SOURCE_DIR)/string_util.c

TEST_DIR = "test"

TEST_SOURCES = \
			   $(TEST_DIR)/chapter_test.c \
			   $(TEST_DIR)/parser_test.c \
			   $(TEST_DIR)/pretty_printer_test.c \
			   $(TEST_DIR)/stream_test.c \
			   $(TEST_DIR)/string_util_test.c \
			   $(TEST_DIR)/unit_testing.c

all:
	$(CC) $(CFLAGS) -o $(BINARY) $(SOURCE_FILES) -lcmark -lreadline

clean:
	rm -fv $(BINARY) $(TEST_DIR)/$(TEST_BINARY)

debug_tests:
	CK_FORK=no gdb $(TEST_DIR)/$(TEST_BINARY)

tags:
	ctags --language-force=C --C-kinds=+p /usr/include/cmark.h *.c

test:
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$(TEST_BINARY) \
		$(TEST_SOURCES) -lcmark -lcheck
	valgrind -q --exit-on-first-error=yes --error-exitcode=1 ./$(TEST_DIR)/$(TEST_BINARY)

test_leaks:
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$(TEST_BINARY) \
		$(TEST_SOURCES) -lcmark -lcheck
	valgrind ./test/check_mdtoc |& grep "lost"

docker_build:
	docker build -t mdtoc-image .docker

docker_run:
	docker run --rm -it -v $(shell pwd):/mdtoc mdtoc-image

install:
	install -v -m 755 ./mdtoc /usr/local/bin/mdtoc

install_latest_release:
	wget https://github.com/bfoersterling/mdtoc/releases/latest/download/mdtoc -O /tmp/mdtoc
	sudo install -v -m 755 /tmp/mdtoc /usr/local/bin/.
