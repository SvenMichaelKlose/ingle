readst  = $ffb7
setlf   = $ffba
setnam  = $ffbd
open    = $ffc0
close   = $ffc3
chkin   = $ffc6
clrchn  = $ffcc
chrin   = $ffcf
clall   = $ffe7

gopen:
    jsr overtake

    lda #2
    ldx #8
    ldy #0
    jsr setlf

    ; Get length of file name and set it.
    ldy #0
l:  lda (s),y
    beq +n
    iny
    jmp -l
n:  tya
    ldx s
    ldy @(++ s)
    jsr setnam

    jsr open
    bcs +error

    ldx #2
    jsr chkin

    php
    jsr resume
    plp
    rts

error:
    jsr resume
    sec
    rts

read:
    jsr overtake

    jsr readst
    bne +eof

    jsr chrin
    pha
    lda $90
    cmp #1   ; set carry when ST>0 (i.e., <>0!)
    pla      ; keep carry, and possibly set Z flag for byte=0
    jmp resume
eof:
    jsr resume
    sec
    rts


    ; Get size of block.
readw:
    jsr read
    bcs +e
    sta c
    jsr read
    bcs +e
    sta @(++ c)
    rts

readm:
    jsr readw

readn:
    inc @(++ c)

l:  jsr read
    bcs +done

    ldy #0
    sta (d),y

    ; Step to next destination address.
    inc d
    bcc +n
    inc @(++ d)
n:

    ; Decrement counter and check if done.
    dec c
    bne -l
    dec @(++ c)
    bne -l

done:
    ldy #0
    sta (d),y
    clc
    rts

e:  sec
    rts

error:
gclose:
    jsr overtake
    jsr clrchn
    lda #2
    jsr close
    jmp resume
