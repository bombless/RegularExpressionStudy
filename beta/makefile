CC=g++
TARGET=yaree
DIR_ALPHA=../alpha/
DIR_BETA=./
NAME=YetAnotherRegularExpressionEngine
SRC_FILE=$(NAME).cpp
SOURCES=unittest.cpp $(DIR_ALPHA)$(SRC_FILE) $(DIR_BETA)$(SRC_FILE)
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES)
test: $(TARGET)
	./yaree
