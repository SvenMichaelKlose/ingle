.export putchar_fixed
.importzp xpos, scr, font
.import calcscr

    .zeropage

tmp:    .res 1
tmp2:   .res 1

    .code

.proc putchar_fixed
    ; Calculate character address.
    ldy #0
    sty tmp2
    asl
    rol tmp2
    asl
    rol tmp2
    asl
    rol tmp2
    clc
    adc font
    sta tmp
    lda tmp2
    adc font+1
    sta tmp2

    jsr calcscr

    ldy #7
    lda xpos
    and #4
    bne l2

l1: lda (tmp),y
    asl
    asl
    asl
    asl
    ora (scr),y
    sta (scr),y
    dey
    bpl l1
    bmi done

l2: lda (tmp),y
    ora (scr),y
    sta (scr),y
    dey
    bpl l2

done:
    lda xpos
    clc
    adc #4
    sta xpos

    rts
.endproc
