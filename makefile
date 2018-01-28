CC=gcc
FLAGS=-std=c99
EXEC=project
EXEC_FOLDER=bin
RELEASE_FOLDER=release
DEBUG_FOLDER=debug
OBJ_FOLDER=obj
BIN_FOLDER=bin
SRC_FOLDER=src
TESTS_FOLDER=tests

OBJ_DEBUG_FOLDER=$(OBJ_FOLDER)/$(DEBUG_FOLDER)
OBJ_RELEASE_FOLDER=$(OBJ_FOLDER)/$(RELEASE_FOLDER)

BIN_DEBUG_FOLDER=$(BIN_FOLDER)/$(DEBUG_FOLDER)
BIN_RELEASE_FOLDER=$(BIN_FOLDER)/$(RELEASE_FOLDER)

.DEFAULT_GOAL := debug

debug-target:  install-debug ArgsParser.o Batch.o BatchList.o Checker.o Config.o helpers.o Instance.o main.o Sequence.o Solution.o TabuList.o    BatchTests.o BatchListTests.o ConfigTests.o helpersTests.o InstanceTests.o SequenceTests.o SolutionTests.o TabuListTests.o TestsRunner.o
	$(CC) $(FLAGS) $(ARGS) -o $(BIN_FOLDER)/$(TARGET)/$(EXEC) $(OBJ_FOLDER)/$(TARGET)/ArgsParser.o $(OBJ_FOLDER)/$(TARGET)/Batch.o $(OBJ_FOLDER)/$(TARGET)/BatchList.o $(OBJ_FOLDER)/$(TARGET)/Checker.o $(OBJ_FOLDER)/$(TARGET)/Config.o $(OBJ_FOLDER)/$(TARGET)/helpers.o $(OBJ_FOLDER)/$(TARGET)/Instance.o $(OBJ_FOLDER)/$(TARGET)/main.o $(OBJ_FOLDER)/$(TARGET)/Sequence.o $(OBJ_FOLDER)/$(TARGET)/Solution.o $(OBJ_FOLDER)/$(TARGET)/TabuList.o $(OBJ_FOLDER)/$(TARGET)/BatchTests.o $(OBJ_FOLDER)/$(TARGET)/BatchListTests.o $(OBJ_FOLDER)/$(TARGET)/ConfigTests.o $(OBJ_FOLDER)/$(TARGET)/helpersTests.o $(OBJ_FOLDER)/$(TARGET)/InstanceTests.o $(OBJ_FOLDER)/$(TARGET)/SequenceTests.o $(OBJ_FOLDER)/$(TARGET)/SolutionTests.o $(OBJ_FOLDER)/$(TARGET)/TabuListTests.o $(OBJ_FOLDER)/$(TARGET)/TestsRunner.o

release-target: install-debug ArgsParser.o Batch.o BatchList.o Checker.o Config.o helpers.o Instance.o main.o Sequence.o Solution.o TabuList.o    
	$(CC) $(FLAGS) $(ARGS) -o $(BIN_FOLDER)/$(TARGET)/$(EXEC) $(OBJ_FOLDER)/$(TARGET)/ArgsParser.o $(OBJ_FOLDER)/$(TARGET)/Batch.o $(OBJ_FOLDER)/$(TARGET)/BatchList.o $(OBJ_FOLDER)/$(TARGET)/Checker.o $(OBJ_FOLDER)/$(TARGET)/Config.o $(OBJ_FOLDER)/$(TARGET)/helpers.o $(OBJ_FOLDER)/$(TARGET)/Instance.o $(OBJ_FOLDER)/$(TARGET)/main.o $(OBJ_FOLDER)/$(TARGET)/Sequence.o $(OBJ_FOLDER)/$(TARGET)/Solution.o $(OBJ_FOLDER)/$(TARGET)/TabuList.o 


ArgsParser.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/ArgsParser.c -o $(OBJ_FOLDER)/$(TARGET)/ArgsParser.o

Batch.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Batch.c -o $(OBJ_FOLDER)/$(TARGET)/Batch.o

BatchList.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/BatchList.c -o $(OBJ_FOLDER)/$(TARGET)/BatchList.o

Checker.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Checker.c -o $(OBJ_FOLDER)/$(TARGET)/Checker.o

Config.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Config.c -o $(OBJ_FOLDER)/$(TARGET)/Config.o

helpers.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/helpers.c -o $(OBJ_FOLDER)/$(TARGET)/helpers.o

Instance.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Instance.c -o $(OBJ_FOLDER)/$(TARGET)/Instance.o

main.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/main.c -o $(OBJ_FOLDER)/$(TARGET)/main.o

Sequence.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Sequence.c -o $(OBJ_FOLDER)/$(TARGET)/Sequence.o

Solution.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/Solution.c -o $(OBJ_FOLDER)/$(TARGET)/Solution.o

TabuList.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(SRC_FOLDER)/TabuList.c -o $(OBJ_FOLDER)/$(TARGET)/TabuList.o


TestsRunner.o:
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/TestsRunner.c -o $(OBJ_FOLDER)/$(TARGET)/TestsRunner.o

BatchTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/BatchTests.c -o $(OBJ_FOLDER)/$(TARGET)/BatchTests.o

BatchListTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/BatchListTests.c -o $(OBJ_FOLDER)/$(TARGET)/BatchListTests.o

ConfigTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/ConfigTests.c -o $(OBJ_FOLDER)/$(TARGET)/ConfigTests.o

helpersTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/helpersTests.c -o $(OBJ_FOLDER)/$(TARGET)/helpersTests.o

InstanceTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/InstanceTests.c -o $(OBJ_FOLDER)/$(TARGET)/InstanceTests.o

SequenceTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/SequenceTests.c -o $(OBJ_FOLDER)/$(TARGET)/SequenceTests.o

SolutionTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/SolutionTests.c -o $(OBJ_FOLDER)/$(TARGET)/SolutionTests.o

TabuListTests.o : 
	$(CC) $(ARGS) $(FLAGS)  -c $(TESTS_FOLDER)/TabuListTests.c -o $(OBJ_FOLDER)/$(TARGET)/TabuListTests.o



.PHONY: debug release all install clean install-debug install-release valgrind-debug valgrind-release run-debug run-release help

all:
	make debug

debug: 
	@make install-debug
	@make ARGS="-g -Wall -Wextra" TARGET="debug" SOURCE="debug" debug-target

release: 
	@make install-release
	@make ARGS="-O3 -DNDEBUG" TARGET="release" SOURCE="release" release-target

install-release:
	@mkdir -p $(OBJ_RELEASE_FOLDER) $(BIN_RELEASE_FOLDER)

install-debug:
	@mkdir -p $(OBJ_DEBUG_FOLDER) $(BIN_DEBUG_FOLDER)

clean:
	@rm -vrf $(OBJ_FOLDER) $(BIN_FOLDER)

valgrind-debug: debug
	@valgrind --leak-check=yes --show-leak-kinds=all $(VALGRIND_OPTION) $(BIN_DEBUG_FOLDER)/$(EXEC) $(ARGS)
	
valgrind-release: release
	@valgrind --leak-check=yes --show-leak-kinds=all $(VALGRIND_OPTION) $(BIN_DEBUG_FOLDER)/$(EXEC) $(ARGS)

run-debug: debug
	@$(BIN_DEBUG_FOLDER)/$(EXEC) $(ARGS)

run-release: release
	@$(BIN_RELEASE_FOLDER)/$(EXEC) $(ARGS)

help:
	@echo "\t- make debug\t\tcompile in debug mode\n"
	@echo "\t- make release\t\tcompile in release mode\n"
	@echo "\t- make clean\t\tclean the workspace (bin and obj folders)\n"
	@echo "\t- valgrind-debug\tcompile in debug mode and run valgrind."
	@echo "\t\t\t\tUse VALGRIND_OPTION=\"...\" to pass options to valgrind, and ARGS=\"...\" to send options to the program\n"
	@echo "\t- valgrind-release\tcompile in release mode and run valgrind."
	@echo "\t\t\t\tUse VALGRIND_OPTION=\"...\" to pass options to valgrind, and ARGS=\"...\" to send options to the program\n"
	@echo "\t- run-debug\t\tcompile in debug mode and run the program. "
	@echo "\t\t\t\tUse ARGS=\"...\" to send options to the program\n"
	@echo "\t- run-release\t\tcompile in release mode and run the program. "
	@echo "\t\t\t\tUse ARGS=\"...\" to send options to the program\n"
	@echo "\t- make\t\t\tsame as 'make debug'\n"
	@echo "\t- make all\t\tsame as 'make debug'\n"
	@echo "\t- make help\t\tdisplay help\n"