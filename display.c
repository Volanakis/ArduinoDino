#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "delay.h"

#define LCD_RS_PORT PORTB
#define LCD_RS_DDR  DDRB
#define LCD_RS_PIN  PB4

#define LCD_E_PORT  PORTB
#define LCD_E_DDR   DDRB
#define LCD_E_PIN   PB3

#define LCD_D4_PORT PORTD
#define LCD_D4_DDR  DDRD
#define LCD_D4_PIN  PD5

#define LCD_D5_PORT PORTD
#define LCD_D5_DDR  DDRD
#define LCD_D5_PIN  PD4

#define LCD_D6_PORT PORTD
#define LCD_D6_DDR  DDRD
#define LCD_D6_PIN  PD3

#define LCD_D7_PORT PORTD
#define LCD_D7_DDR  DDRD
#define LCD_D7_PIN  PD2

static void pulse_enable(void) {
    LCD_E_PORT |= _BV(LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~_BV(LCD_E_PIN);
    _delay_us(1);
}

static void write_nibble(uint8_t nibble) {
    if (nibble & 0x01) LCD_D4_PORT |= _BV(LCD_D4_PIN);
    else               LCD_D4_PORT &= ~_BV(LCD_D4_PIN);

    if (nibble & 0x02) LCD_D5_PORT |= _BV(LCD_D5_PIN);
    else               LCD_D5_PORT &= ~_BV(LCD_D5_PIN);

    if (nibble & 0x04) LCD_D6_PORT |= _BV(LCD_D6_PIN);
    else               LCD_D6_PORT &= ~_BV(LCD_D6_PIN);

    if (nibble & 0x08) LCD_D7_PORT |= _BV(LCD_D7_PIN);
    else               LCD_D7_PORT &= ~_BV(LCD_D7_PIN);
}

static void lcd_send_byte(uint8_t rs, uint8_t data) {
    if (rs) LCD_RS_PORT |= _BV(LCD_RS_PIN);
    else    LCD_RS_PORT &= ~_BV(LCD_RS_PIN);

    write_nibble(data >> 4);
    pulse_enable();
    write_nibble(data & 0x0F);
    pulse_enable();
}

void lcd_command(uint8_t cmd) {
    _delay_us(50);
    lcd_send_byte(0, cmd);
}

void lcd_data(uint8_t data) {
    _delay_us(50);
    lcd_send_byte(1, data);
}

void lcd_write_byte(uint8_t data) {
    lcd_data(data);
}

void lcd_init(void) {
    LCD_RS_DDR |= _BV(LCD_RS_PIN);
    LCD_E_DDR  |= _BV(LCD_E_PIN);
    LCD_D4_DDR |= _BV(LCD_D4_PIN);
    LCD_D5_DDR |= _BV(LCD_D5_PIN);
    LCD_D6_DDR |= _BV(LCD_D6_PIN);
    LCD_D7_DDR |= _BV(LCD_D7_PIN);
    LCD_RS_PORT &= ~_BV(LCD_RS_PIN);
    LCD_E_PORT  &= ~_BV(LCD_E_PIN);

    delay_ms(20);

    write_nibble(0x03);
    pulse_enable();
    delay_ms(5);

    write_nibble(0x03);
    pulse_enable();
    _delay_us(150);

    write_nibble(0x03);
    pulse_enable();
    _delay_us(150);

    write_nibble(0x02);
    pulse_enable();
    _delay_us(150);

    lcd_command(0x28);
    lcd_command(0x08);
    lcd_command(0x01);
    delay_ms(2);
    lcd_command(0x06);
    lcd_command(0x0C);
}

void lcd_clear(void) {
    lcd_command(0x01);
    delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t addr[] = { 0x00, 0x40 };
    lcd_command(0x80 | (addr[row] + col));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_data((uint8_t)*str);
        str++;
    }
}

void lcd_print_int(int16_t val) {
    if (val < 0) {
        lcd_data('-');
        val = -val;
    }
    char buf[8];
    int8_t i = 7;
    buf[7] = '\0';
    if (val == 0) {
        lcd_data('0');
        return;
    }
    while (val > 0 && i > 0) {
        i--;
        buf[i] = '0' + (val % 10);
        val /= 10;
    }
    while (buf[i]) {
        lcd_data((uint8_t)buf[i]);
        i++;
    }
}

void lcd_create_char(uint8_t location, const uint8_t *bitmap) {
    lcd_command(0x40 | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        lcd_data(bitmap[i]);
    }
    lcd_set_cursor(0, 0);
}
