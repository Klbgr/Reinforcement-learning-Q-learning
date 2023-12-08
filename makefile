SOURCE := src
BUILD := build
DOXYGEN := doxygen

all : main doxygen

main: $(BUILD) $(BUILD)/main.o $(BUILD)/q_learning.o $(BUILD)/gui.o $(BUILD)/params.o
	gcc $(BUILD)/*.o -o main -lSDL2 -lSDL2_ttf -lm

doxygen:
	doxygen ./generate_doxygen

clean :
	rm -rf $(BUILD) main $(DOXYGEN)

$(BUILD) :
	mkdir -p $(BUILD)

$(BUILD)/main.o: $(SOURCE)/main.c
	gcc -c $(SOURCE)/main.c -o $(BUILD)/main.o

$(BUILD)/q_learning.o: $(SOURCE)/q_learning.c $(SOURCE)/q_learning.h
	gcc -c $(SOURCE)/q_learning.c -o $(BUILD)/q_learning.o

$(BUILD)/gui.o: $(SOURCE)/gui.c $(SOURCE)/gui.h
	gcc -c $(SOURCE)/gui.c -o $(BUILD)/gui.o

$(BUILD)/params.o: $(SOURCE)/params.c $(SOURCE)/params.h
	gcc -c $(SOURCE)/params.c -o $(BUILD)/params.o