CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++20

APP_NAME = chat-server

OBJ_PATH = obj
SRC_PATH = src


APP_SRCS_FILES = $(wildcard $(SRC_PATH)/*.cpp)
APP_OBJS_FILES = $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(APP_SRCS_FILES))


all: app

app: $(OBJ_PATH) $(APP_OBJS_FILES)
	$(CC) $(CFLAGS) $(APP_OBJS_FILES) -o $(APP_NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_PATH):
	mkdir $@



test: $(TEST_BIN_PATH) $(OBJ_PATH) $(APP_OBJS_FILES) $(TESTS_BINS)
	for test in $(TESTS_BINS) ; do ./$$test ; done

clean:
	rm -r obj