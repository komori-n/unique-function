CC      = g++ -O3 -std=c++17
CFLAGS  = -Wall -Wextra -I../include
SRC_DIR = ./src
OBJ_DIR = ./obj
SOURCES = $(shell ls $(SRC_DIR)/*.cpp)
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
OF_TARGET = onetime_function_examples.out
UF_TARGET = unique_function_examples.out
TARGET = $(OF_TARGET) $(UF_TARGET)
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(UF_TARGET): $(OBJ_DIR)/unique_function_examples.o
	$(CC) -o $@ $< -lpthread

$(OF_TARGET): $(OBJ_DIR)/onetime_function_examples.o
	$(CC) -o $@ $< -lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d $(OBJ_DIR) ]; \
		then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
	fi
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean
