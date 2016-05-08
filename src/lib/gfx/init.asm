.export gfx_init

.import init_bitmap_mode, reset_region
.importzp c_setzb
.importzp font, font_space_size, font_compression, do_compress_font_gaps
.import exec_script

.code

.proc gfx_init
    lda #<exec_script
    sta $316
    lda #>exec_script
    sta $317

    lda #0
    ldx #1
    ldy #3
    jsr init_bitmap_mode

    jsr reset_region

    brk
    .byte c_setzb, font, $88
    .byte c_setzb, do_compress_font_gaps, 1
    .byte c_setzb, font_compression, 2
    .byte c_setzb, font_space_size, 3
    .byte 0
    rts
.endproc
