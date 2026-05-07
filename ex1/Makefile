C_COMPILER   := mpicc
COMMON_FLAGS := -std=c99 -O2 -Wall -Wno-unused-function -Wno-unused-variable

C_FLAGS      := $(COMMON_FLAGS)
LD_FLAGS     := $(COMMON_FLAGS) -lm

BIN       := build/mpi_matrix_multiply.out
C_FILES   := $(wildcard src/*.c)
H_FILES   := $(wildcard src/*.h)
OBJ_FILES := $(C_FILES:src/%.c=build/%.o)


all :: build


.PHONY: run
run :: run-4

run-% :: PROCESS_COUNT = $(@:run-%=%)
run-% :: $(BIN)
	mpirun -n $(PROCESS_COUNT) $(BIN)

.PHONY: build
build :: $(BIN)

.PHONY: clean
clean ::
	@rm -rf $(BIN) $(OBJ_FILES)


$(BIN) :: $(OBJ_FILES) $(H_FILES)
	$(C_COMPILER) $(OBJ_FILES) $(LD_FLAGS) -o $@

build/%.o :: src/%.c $(H_FILES)
	$(C_COMPILER) -c $< $(C_FLAGS) -o $@
