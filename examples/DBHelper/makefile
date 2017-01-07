CC := clang++ 
DEBUG :=
OPT := -O2
CFLAGS := -std=c++14 -pthread
INCLUDES :=
LFLAGS := -lsqlite3
LIBS := 
SRC := $(wildcard *.cpp)
OBJ := $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
EXC := out
OBJ_DIR := obj/
RM := -@\rm -f
RM_DIR := -@\rm -rf

.PHONY: all
all: resources $(EXC)
run: resources runner

resources:
	@mkdir -p obj

runner: $(EXC)
	./$(EXC)

$(EXC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(LIBS)

$(LIB_OUT): $(OBJ)
	$(LIB_CMD) $@ $^
	mv $(LIB_OUT) $(LIB_DIR)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean

clean:
	$(RM_DIR) $(OBJ_DIR)
	$(RM) $(EXC)
	$(RM) example.db
