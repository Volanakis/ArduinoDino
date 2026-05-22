#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

#define MENU_SIZE      2
#define LCD_COLUMN     16
#define TREE_CHAR      6
#define DINO_CHAR      7
#define MAX_SCORES     20
#define NAME_LEN       3

extern bool isPlaying;
extern bool isShowScore;
extern bool isDinoOnGround;
extern int16_t score;
extern uint8_t scoreListSize;
extern char scoreList[MAX_SCORES][20];

void start_game(void);
void handle_game(void);
void handle_game_over(void);
void save_score(void);
void show_tree(int16_t position);
void define_dino_position(void);
void put_dino_on_ground(void);
void put_dino_on_air(void);

#endif
