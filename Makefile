SRC_DIR = .
TARGET = 20091631
ARGUMENTS = $(filter-out arm intel build.core build.modules, $(MAKECMDGOALS))

#IS_BUILD = $(findstring build, $(MAKECMDGOALS))
#IS_TESTS = $(findstring tests, $(MAKECMDGOALS))
#IS_CLEAN = $(findstring clean, $(MAKECMDGOALS))
ifeq "$(IS_BUILD)$(IS_TESTS)$(IS_CLEAN)" ""
IS_BUILD = build  #default
endif

.PHONY: default
default:
	make arm build

.PHONY: arm
arm: 
	@# usage
	@# $ make (arm) build
	@# $ make (arm) tests
	ln -s -f `which arm-none-linux-gnueabi-gcc` Compiler
	@make $(IS_BUILD) $(IS_TESTS) $(IS_CLEAN) $(ARGUMENTS)

.PHONY: intel
intel: 
	@# usage
	@# $ make intel build
	@# $ make intel tests
	ln -s -f `which gcc` Compiler
	@make $(IS_BUILD) $(IS_TESTS) $(IS_CLEAN) $(ARGUMENTS)

.PHONY: build
build: build.core build.modules .mkdir.o
	$(CC) -g -Wall -o $(OBJS_DIR)/main.o -c main.c -I$(SRC_DIR) -static
	$(CC) -g -Wall -o $(DEST_DIR)/20091631.out $(OBJS_DIR)/*/*.o $(OBJS_DIR)/main.o -static
	@#cp $(DEST_DIR)/20091631.out $(DEST_DIR)/../20091631.out

.PHONY: tests
tests: build.core build.modules


build.core:
	@make -C $(SRC_DIR)/core $(ARGUMENTS)

build.modules: core
	@make -C $(SRC_DIR)/modules $(ARGUMENTS)

include $(SRC_DIR)/Rules.mk 
