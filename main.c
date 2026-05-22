#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "game.h"
#include "delay.h"

extern const uint8_t dino_bitmap[8];
extern const uint8_t tree_bitmap[8];

static int16_t currentIndexMenu = 0;

static void handle_menu(void);
static void show_score(void);
static void print_score(int16_t index, int16_t lastIndex);

void setup(void) {
    lcd_init();
    lcd_create_char(DINO_CHAR, dino_bitmap);
    lcd_create_char(TREE_CHAR, tree_bitmap);

    DDRB &= ~_BV(PB0);
    DDRB &= ~_BV(PB1);
    PORTB |= _BV(PB0);
    PORTB |= _BV(PB1);
}

void loop(void) {
    lcd_clear();
    handle_menu();
    delay_ms(300);
}

static void handle_menu(void) {
    const char *menu[MENU_SIZE] = { "START", "SCORE" };

    for (int16_t i = 0; i < MENU_SIZE; i++) {
        if (i == currentIndexMenu) {
            lcd_set_cursor(0, i);
            lcd_print("-> ");
        }
        lcd_set_cursor(3, i);
        lcd_print(menu[i]);
    }

    if ((PINB & _BV(PB1)) == 0) {
        currentIndexMenu = (currentIndexMenu == 0) ? 1 : 0;
    }

    if ((PINB & _BV(PB0)) == 0) {
        if (currentIndexMenu == 0) {
            start_game();
        } else {
            show_score();
        }
    }
}

static void show_score(void) {
    isShowScore = true;
    delay_ms(200);

    int16_t currentIndex = 0;
    int16_t lastIndex = scoreListSize - 1;

    print_score(currentIndex, lastIndex);

    while (isShowScore) {
        if ((PINB & _BV(PB1)) == 0) {
            currentIndex = (currentIndex < lastIndex) ? currentIndex + 1 : 0;
            print_score(currentIndex, lastIndex);
        }
        if ((PINB & _BV(PB0)) == 0) {
            isShowScore = false;
        }
        delay_ms(200);
    }
}

static void print_score(int16_t index, int16_t lastIndex) {
    lcd_clear();

    if (lastIndex == -1) {
        lcd_print("NO SCORE");
    } else {
        lcd_print(scoreList[index]);
        if (index < lastIndex) {
            lcd_set_cursor(0, 1);
            lcd_print(scoreList[index + 1]);
        }
    }
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}
