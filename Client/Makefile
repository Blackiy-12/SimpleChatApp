CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++20

APP_NAME = chat-client

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

clean:
	rm -r obj $(APP_NAME) test/bin