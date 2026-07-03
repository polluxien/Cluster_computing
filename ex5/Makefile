# ================================================================================
# Parameters

MPI_COUNT ?= 2
OMP_COUNT ?= 4


# ================================================================================
# Compilation

CC      = gcc
CFLAGS  = -std=c99 -fopenmp -g -Wall -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS = -lm


# ================================================================================
# Files

DIR_CODE  = src
DIR_BUILD = dist

C_FILES = $(wildcard $(DIR_CODE)/*.c)
H_FILES = $(wildcard $(DIR_CODE)/*.h)
TARGET  = $(DIR_BUILD)/n-body-sim.out
LOGFILE = runlog.csv
ZIPFILE = source.zip


# ================================================================================
# Targets

$(TARGET): OMPI_CC = "$(CC)"
$(TARGET): $(DIR_BUILD) $(C_FILES) $(H_FILES)
	@echo "MPICC $@"
	@mpicc $(CFLAGS) $(C_FILES) -o $@ $(LDFLAGS)

$(DIR_BUILD):
	@echo "MKDIR $@"
	@mkdir -p $@

$(LOGFILE):
	@echo "TOUCH $@"
	@[ ! -f "$@" ] && touch $@ || true

$(ZIPFILE):
	@echo "ZIP $@"
	@zip -r $@ src Makefile runlog.csv


# ================================================================================
# Commands

.PHONY: build
build: $(TARGET)


.PHONY: clean
clean:
	@echo "RM dist"
	@rm -rf dist

.PHONY: run
run: OMP_NUM_THREADS = $(OMP_COUNT)
run: $(TARGET) $(LOGFILE)
	@echo "RUN (MPI_COUNT = $(MPI_COUNT), OMP_COUNT = $(OMP_COUNT))"
	@mpirun -n $(MPI_COUNT) $(TARGET) >> $(LOGFILE)

.PHONY: zip
zip: clean $(ZIPFILE)

