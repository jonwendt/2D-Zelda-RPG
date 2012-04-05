GAME_FILES = main.cpp main.h Object.cpp Object.h Enemy.cpp Enemy.h

Game: $(GAME_FILES)
	g++ -g -o $@ $(GAME_FILES) `allegro-config --cflags --libs`
