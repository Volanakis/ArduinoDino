#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "display.h"
#include "delay.h"

#define BUTTON_ENTER_PIN  PB0
#define BUTTON_SELECT_PIN PB1

bool isPlaying = false;
bool isShowScore = false;
bool isDinoOnGround = true;
int16_t score = 0;
uint8_t scoreListSize = 0;
char scoreList[MAX_SCORES][20];

static uint8_t read_button_enter(void) {
    return (PINB & _BV(BUTTON_ENTER_PIN)) != 0;
}

static uint8_t read_button_select(void) {
    return (PINB & _BV(BUTTON_SELECT_PIN)) != 0;
}

void start_game(void) {
    isPlaying = true;
    while (isPlaying) {
        handle_game();
    }
}

void handle_game(void) {
    lcd_clear();
    uint8_t buttonPressedTimes = 0;
    int16_t secondPosition = 4 + (rand() % 5);
    int16_t thirdPosition = 4 + (rand() % 5);
    int16_t firstTreePosition = LCD_COLUMN;
    int16_t columnValueToStopMoveTrees = -(secondPosition + thirdPosition);

    for (; firstTreePosition >= columnValueToStopMoveTrees; firstTreePosition--) {
        lcd_set_cursor(13, 0);
        lcd_print_int(score);

        define_dino_position();

        int16_t secondTreePosition = firstTreePosition + secondPosition;
        int16_t thirdTreePosition = secondTreePosition + thirdPosition;

        show_tree(firstTreePosition);
        show_tree(secondTreePosition);
        show_tree(thirdTreePosition);

        if (isDinoOnGround) {
            if (firstTreePosition == 1 || secondTreePosition == 1 || thirdTreePosition == 1) {
                handle_game_over();
                break;
            }
            buttonPressedTimes = 0;
        } else {
            if (buttonPressedTimes > 3) {
                score -= 3;
            }
            buttonPressedTimes++;
        }

        score++;
        delay_ms(500);
    }
}

void handle_game_over(void) {
    lcd_clear();
    lcd_print("GAME OVER");
    lcd_set_cursor(0, 1);
    lcd_print("SCORE: ");
    lcd_print_int(score);
    delay_ms(2000);
    save_score();
}

void save_score(void) {
    lcd_clear();
    char nick[NAME_LEN + 1] = {0};
    uint8_t nameSize = 0;
    uint8_t alphabetCurrentIndex = 0;

    lcd_print("TYPE YOUR NAME");

    while (nameSize != NAME_LEN) {
        lcd_set_cursor(nameSize, 1);
        lcd_data('A' + alphabetCurrentIndex);

        if (read_button_select() == 0) {
            alphabetCurrentIndex = (alphabetCurrentIndex != 25) ? alphabetCurrentIndex + 1 : 0;
        }

        if (read_button_enter() == 0) {
            nick[nameSize] = 'A' + alphabetCurrentIndex;
            nameSize++;
            alphabetCurrentIndex = 0;
        }

        delay_ms(300);
    }

    scoreList[scoreListSize][0] = '\0';
    strcat(scoreList[scoreListSize], nick);
    strcat(scoreList[scoreListSize], " ");
    char buf[8];
    itoa(score, buf, 10);
    strcat(scoreList[scoreListSize], buf);

    scoreListSize++;
    isPlaying = false;
    score = 0;
}

void show_tree(int16_t position) {
    lcd_set_cursor(position, 1);
    lcd_write_byte(TREE_CHAR);
    lcd_set_cursor(position + 1, 1);
    lcd_print(" ");
}

void define_dino_position(void) {
    if (read_button_enter() != 0) {
        put_dino_on_ground();
    } else {
        put_dino_on_air();
    }
}

void put_dino_on_ground(void) {
    lcd_set_cursor(1, 1);
    lcd_write_byte(DINO_CHAR);
    lcd_set_cursor(1, 0);
    lcd_print(" ");
    isDinoOnGround = true;
}

void put_dino_on_air(void) {
    lcd_set_cursor(1, 0);
    lcd_write_byte(DINO_CHAR);
    lcd_set_cursor(1, 1);
    lcd_print(" ");
    isDinoOnGround = false;
}
