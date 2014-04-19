DEST_DIR = $(SRC_DIR)/built
OBJS_DIR = $(DEST_DIR)/objs
TESTS_DIR = $(DEST_DIR)/tests
CFLAGS = -g -static -I$(SRC_DIR) -Wall
CC = $(SRC_DIR)/Compiler

.PHONY: clean
clean:
	@rm -rf $(DEST_DIR) $(CC)
	@echo "Cleaned."

$(addsuffix .o, $(TARGET)): %.o: %.c %.h .mkdir.o
	$(CC) -c $(basename $@).c $(CFLAGS) -o $(OBJS_DIR)/$(PART_NAME)/$(basename $@).o

$(addsuffix .test, $(TARGET)): %.test: %.c %.h .mkdir.test
	$(CC)    $(basename $@).c $(CFLAGS) -o $(TESTS_DIR)/$(basename $@).test -D$(basename $@)_test $(TEST_INCLUDING)

.PHONY: .mkdir.o
.mkdir.o:
	@mkdir -p $(OBJS_DIR)/$(PART_NAME)

.PHONY: .mkdir.test
.mkdir.test:
	@mkdir -p $(TESTS_DIR)
