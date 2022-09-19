.export init_alloc, alloc_bank, free_bank, lock_bank

.import set_banks, restore_banks, restore_banks_and_y

num_banks = 1024 / 8    ; TODO: Use detection for VIC-MIDI.

.bss

ram_map:            .res num_banks / 8
last_bank_in_map:   .res 1
free_banks:         .res 1
tmp:                .res 1

.data

free_masks:
    .byte %00000001
    .byte %00000010
    .byte %00000100
    .byte %00001000
    .byte %00010000
    .byte %00100000
    .byte %01000000
    .byte %10000000

alloc_masks:
    .byte %11111110
    .byte %11111101
    .byte %11111011
    .byte %11110111
    .byte %11101111
    .byte %11011111
    .byte %10111111
    .byte %01111111

.code

.proc init_alloc
    lda #%00000011
    jsr set_banks

    ldx #num_banks / 8
    lda #$7f
    sta ram_map - 1,x
    dex
    lda #$ff
l1: sta ram_map - 1,x
    dex
    bne l1

    lda #0
    sta ram_map

    lda #num_banks - 1
    sta free_banks
    lda #0
    sta last_bank_in_map
    rts
.endproc

.proc alloc_bank
    txa
    pha
    lda $9ff4
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    ldx free_banks
    beq all_gone

    ldx last_bank_in_map
again:
    lda ram_map,x
    bne found
    inx
    bpl again
    ldx #0
    beq again       ; (jmp)

found:
    ldy #0
next_bit:
    lsr
    bcs got_it
    iny
    bne next_bit    ; (jmp)
    
got_it:
    lda ram_map,x
    and alloc_masks,y
    sta ram_map,x
    txa
    asl
    asl
    asl
    sty tmp
    clc
    adc tmp
    tay
    dec free_banks

return:
    pla
    sta $9ff1
    pla
    sta $9ff5
    pla
    sta $9ff4
    pla
    tax
    tya
    rts

all_gone:
    sec
    bcs return      ; (jmp)
.endproc

.proc free_bank
    tax
    lda $9ff4
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    txa
    and #%00000111
    tay
    txa
    lsr
    lsr
    lsr
    tax
    lda ram_map,x
    and alloc_masks,y
    bne error
    lda free_masks,y
    ora ram_map,x
    sta ram_map,x

    clc
return:
    jmp restore_banks

error:
    sec
    bcs return          ; (jmp)
.endproc

.proc lock_bank
    tax
    tya
    pha
    lda $9ff4
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    txa
    and #%00000111
    tay
    txa
    lsr
    lsr
    lsr
    tax

    lda ram_map,x
    and alloc_masks,y
    sta ram_map,x

    jmp restore_banks_and_y
.endproc
