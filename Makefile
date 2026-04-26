
# ==============================================================================
# == Configuration.

PROCESS_COUNT := 4

C_COMPILER   := gcc
COMMON_FLAGS := -std=c99 -Wall -O2 -Wno-unused-function -Wno-unused-variable
C_FLAGS      := $(COMMON_FLAGS) $(shell mpicc --showme:compile)
LD_FLAGS     := $(COMMON_FLAGS) $(shell mpicc --showme:link)

BUILD_DIR  := build
SOURCE_DIR := src

BIN       := $(BUILD_DIR)/mpi_matrix_multiply.out
C_FILES   := $(wildcard $(SOURCE_DIR)/*.c)
H_FILES   := $(wildcard $(SOURCE_DIR)/*.h)
OBJ_FILES := $(C_FILES:src/%.c=build/%.o)


# ==============================================================================
# == Phony targets.

.PHONY: run
run :: $(BIN)
	@printf '             \r'
	@echo '[run]'
	@mpirun -n $(PROCESS_COUNT) $(BIN)
	

.PHONY: build
build :: $(BIN)
	@printf '             \r'
	@echo '[build]'

.PHONY: clean
clean ::
	@printf '             \r'
	@echo '[clean]'
	@rm -rf $(BIN) $(OBJ_FILES)


# ==============================================================================
# == Static rules.

$(BIN) :: $(OBJ_FILES)
	@echo '[link]    "$@"'
	@$(C_COMPILER) $^ $(LD_FLAGS) -o $@

build/%.o :: src/%.c $(HEADERS)
	@echo '[compile] "$@"'
	@$(C_COMPILER) -c $^ $(C_FLAGS) -o $@


