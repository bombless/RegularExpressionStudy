CC=g++
TARGET=yaree
NAME=YetAnotherRegularExpressionEngine
SRC_FILE=$(NAME).cpp
SOURCES=unittest.cpp $(SRC_FILE)
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES)
test: $(TARGET)
	./yaree
