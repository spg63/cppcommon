# Sean Grimes
CC := clang++
CFLAGS := -std=c++14
LIBS :=
INCLUDES := 
LFLAGS := 
SRC := $(wildcard *.cpp)
OBJ := $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
EXC := out
RM := -@\rm -f
RM_DIR := @\rm -rf
LIB := liboutputmodifier.a
LIB_DIR := lib/
OBJ_DIR := obj/
LIB_CMD := ar rvs
OPT := -O2

# Apple specifics vs linux, why can't we all just conform...
OS := $(shell uname)
ifeq ($(OS), Darwin)
override LIBS := -framework OpenCL 
else
override LIBS := -lOpenCL
override CFLAGS := -pthread $(CFLAGS)
endif

.PHONY: all lib run
all: resources $(EXC)
run: resources runner

resources:
	@mkdir -p obj

runner: $(EXC)
	./$(EXC)

$(EXC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(LIBS)

$(LIB): $(OBJ)
	$(LIB_CMD) $@ $^
	mv $(LIB) $(LIB_DIR)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean 

clean:
	$(RM_DIR) $(OBJ_DIR)
	$(RM) $(EXC)
    

