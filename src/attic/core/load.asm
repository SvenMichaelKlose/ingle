; XXX This is intended to go into dev/cbm if there's a working virtual
; file system.
gopen:
    jsr stop_task_switching

    lda #2
    ldx $ba
    ldy #0
    jsr setlfs

    jsr strlen
    tya
    ldx s
    ldy @(++ s)
    jsr setnam

    jsr open
    bcs +error

    ldx #2
    jsr chkin
    bcs +error
    jmp start_task_switching

error:
    jsr set_cbm_error
    jmp start_task_switching

read:
    jsr stop_task_switching

    jsr readst
    bne +eof

    jsr chrin
    bcs -error
    pha
    lda $90
    cmp #1   ; set carry when ST>0 (i.e., <>0!)
    pla      ; keep carry, and possibly set Z flag for byte=0
    jmp start_task_switching
eof:
    sec
    jmp start_task_switching


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
    jsr inc_d

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

gclose:
    jsr stop_task_switching
    jsr clrchn
    lda #2
    jsr close
    jmp start_task_switching
