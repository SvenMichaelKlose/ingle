.export ultimem_burn_byte
.export _ultimem_burn_byte

.import _ultimem_send_command

.importzp s, d, tmp
.import popax

.proc _ultimem_burn_byte
    sta tmp
    jsr popax
    ldy tmp
.endproc

.proc ultimem_burn_byte
    sta s
    stx s+1

    lda #$a0
    jsr _ultimem_send_command

    tya
    ldx #0
    sta (s,x)

    rts
.endproc
