.export _launch
.importzp s, d, c, tmp, tmp2
.import popax

bstart  = $2b       ; start of BASIC program text
bend    = $2d       ; end of basic program text
membot  = $282      ; start page of BASIC RAM
memtop  = $284      ; end page of BASIC RAM
screen  = $288      ; start page of text matrix

warmstt = $c7ae     ; BASIC warm start

; void __fastcall__ launch (unsigned start, unsigned size);
.proc _launch
    sta c
    stx c+1
    jsr popax
    sta d
    sta tmp
    stx d+1
    stx tmp2
    lda #0
    sta s

    ; Don't get interrupted.
    sei
    lda #$7f
    sta $911d
    sta $911e

    lda #0      ; Blank screen.
    sta $9002

    ; Copy loaded data starting at bank 8 to RAM via BLK5.
    lda #7
    sta $9ffe
    ldy #0
    ldx c
l4: inc $9ffe
    lda #>$a000
    sta s+1
l:  lda (s),y
    sta (d),y
    inc d
    beq d1
l2: inc s
    beq d2
l3: dex
    cpx #$ff
    bne l
    dec c+1
    lda c+1
    cmp #$ff
    bne l

    lda tmp2
    cmp #>$1000
    bne l5

    ; unexpanded
    lda #%00111100  ; No RAM1/2/3
    sta $9ff1
    lda #0
    sta $9ff2

    lda #>$1e00     ; screen
    ldx #>$1000     ; BASIC
    ldy #>$1e00     ; BASIC end
    bne l6

    ; +24/32/35
l5: lda #>$1000     ; screen
    ldx #>$1200     ; BASIC
    ldy #>$8000     ; BASIC end

l6: sty $c2         ; end of BASIC
    sta screen
    stx membot
    sty memtop

    jsr $ff8a       ; initialize the KERNAL jump vectors
    jsr $fdf9       ; initialize the I/O chips
    jsr $e518       ; initialize the screen
    jsr $e45b       ; initialize jump vectors for BASIC
    jsr $e3a4       ; initialize zero page for BASIC
    lda bstart
    ldy bstart+1
    jsr $c408       ; check memory overlap
    jsr $c659       ; CLR

    jmp warmstt

d1: inc d+1
    bne l2
    beq l2

d2: inc s+1
    lda s+1
    cmp #>$c000
    beq l4
    bne l3
.endproc