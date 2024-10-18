GCC_FLAGS = -std=c11 -Wall -Wextra -Werror
BUILD_DIR = ./build
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
EXE = test.out

# Папки поиска
SRC_DIRS = ./brick_game/tetris ./gui/cli
OBJ_DIR = ./objs
TESTS_DIR := tests
DIST_DIR = Tetris
TETRIS_DIR := brick_game/tetris
LDFLAGS = -lncurses
TEST_OBJ_DIR = ./tests/objs
BUILD_PATH = gcov_report/
REPORT_PATH = $(BUILD_PATH)report/
TEST_C_FILES := $(wildcard $(TEST_DIR)/test_*.c)
OBJECTS := $(TETRIS_DIR)/*.o $(GUI_DIR)/*.o

# Поиск всех файлов .c во всех SRC
SRC_C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Создание списка объектных файлов, сохраняя структуру директорий
OBJS_C_FILES := $(patsubst $(SRC_DIRS)/%.c, $(OBJ_DIR)/%.o, $(SRC_C_FILES))

ifeq ($(shell uname -s), Linux)
	TEST_FLAGS = -lcheck -lpthread -pthread -lm  -lrt -lsubunit
	OPEN = xdg-open
	LDFLAGS := -lncursesw
else
	TEST_FLAGS = -lcheck -lpthread -pthread -lm 
	LDFLAGS := -lncurses
	OPEN = open
endif

all: test gcov_report

rebuild: clean install

# Правило для сборки исполняемого файла
install: $(OBJS_C_FILES)
	mkdir -p $(BUILD_DIR)
	gcc -g $(GCC_FLAGS) -o $(BUILD_DIR)/tetris main.c $(OBJS_C_FILES) $(TEST_FLAGS) $(LDFLAGS)

uninstall:
	rm -rf ./build $(OBJECTS)

start_game:
	./$(BUILD_DIR)/tetris

# цель для открытия html документации
dvi:
	$(OPEN) Documentation.html

# цель для создания дистрибутивного пакета проекта
dist: clean
	@mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a tests $(DIST_DIR)
	cp -a *.c $(DIST_DIR)
	cp -a Documentation.html $(DIST_DIR)
	tar -czf s21_Tetris.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

test:
	@mkdir -p $(BUILD_DIR)
	gcc -DTEST_MODE $(GCFLAGS) -g $(OBJS_C_FILES) $(TESTS_DIR)/*.c -o $(BUILD_DIR)/tetris $(LDFLAGS) $(TEST_FLAGS)
	./$(BUILD_DIR)/tetris


gcov_report: clean
	@mkdir -p $(BUILD_DIR) 
	gcc  -DTEST_MODE $(OBJS_C_FILES) $(TESTS_DIR)/*.c -o $(BUILD_DIR)/tetris $(LDFLAGS) $(TEST_FLAGS) -fprofile-arcs -ftest-coverage
	./$(BUILD_DIR)/tetris
	lcov -t "$(TETRIS_DIR)/tetris.c" -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcno *.gcda

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR) $(TEST_OBJ_DIR) report *.out *.gc* *.info $(BUILD_PATH) *.tar.gz *.txt

docker_build:
	cp ../materials/linters/.clang-format ./
	docker build -t vault:v1 .
	docker create -it --name decimal_vault vault:v1

docker_start:
	docker start -i decimal_vault

docker_delete:
	docker rm decimal_vault
	docker image rm vault:v1

style_test:
	cp ../materials/linters/.clang-format .
	clang-format -n ./brick_game/tetris/*.c ./brick_game/tetris/*.h
	rm .clang-format

format_style:
	cp ../materials/linters/.clang-format .
	clang-format -i ./brick_game/tetris/*.c ./brick_game/tetris/*.h
	rm .clang-format
