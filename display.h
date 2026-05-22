#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void lcd_init(void);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_print_int(int16_t val);
void lcd_create_char(uint8_t location, const uint8_t *bitmap);
void lcd_write_byte(uint8_t data);

#endif
