.section .text

; Dino character bitmap (5x8)
.global dino_bitmap
dino_bitmap:
    .byte 0b00000000
    .byte 0b00000111
    .byte 0b00000101
    .byte 0b10110111
    .byte 0b00011100
    .byte 0b00011111
    .byte 0b00001101
    .byte 0b00001100

; Tree character bitmap (5x8)
.global tree_bitmap
tree_bitmap:
    .byte 0b00000011
    .byte 0b00011011
    .byte 0b00011011
    .byte 0b00011011
    .byte 0b00011011
    .byte 0b00011111
    .byte 0b00001110
    .byte 0b00001110

; Delay approximately N milliseconds at 16MHz
; r24:r25 = milliseconds (16-bit)
.global delay_ms
delay_ms:
    push    r16
    push    r17
    push    r18
1:  ldi     r16, 50
2:  ldi     r17, 200
3:  dec     r17
    brne    3b
    dec     r16
    brne    2b
    sbiw    r24, 1
    brne    1b
    pop     r18
    pop     r17
    pop     r16
    ret
