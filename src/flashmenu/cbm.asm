temporary_bank  = $a000

.export _cbm_read_directory

.importzp s, d, tmp, tmp2, tmp3
.import popax

.include "../core/dev/cbm/kernal.asm"

max_file_name_length = 16
dirent_name     = 0
dirent_length   = max_file_name_length
dirent_type     = dirent_length + 4
dirent_size     = dirent_type + 1

.proc error
    ldx #1
    rts
.endproc

; int __fastcall__ cbm_read_directory (char * pathname, char device);
; s: CBM path name
.proc _cbm_read_directory
    pha
    jsr popax
    sta s
    stx s+1
    jsr strlen
    tya
    ldx s
    ldy s+1
    jsr SETNAM

    pla
    tax
    ldy #$00    ; Read.
    lda #$02    ; Reserved logical file number.
    jsr SETLFS

    jsr OPEN
    bcs error

    ldx #$02
    jsr CHKIN
    bcs error

    ; Skip load address.
    jsr read
    bcs error
    jsr read
    bcs error

    ;; Skip first three lines.
    ldx #3
    ; Skip load address, first address of next line and line number.
m:  ldy #6
l1: jsr read
    bcs error
    dey
    bne l1

    ; Skip line.
    ldy #0
l2: jsr read
    bcs error
    bne l2     ; continue until end of line

    dex
    bne m

    ; Get temporary bank.
    lda #<temporary_bank
    sta d
    lda #>temporary_bank
    sta d+1

next_entry:
    lda d
    sta tmp2
    lda d+1
    sta tmp3

    jsr READST
    bne done

    ; Skip address of next line.
    jsr read
    bcs error2
    jsr read
    bcs error2

    ; Read BASIC line number, which is the size in blocks.
    jsr read
    bcs error2
    ldy #dirent_length
    sta (d),y
    jsr read
    bcs error2
    iny
    sta (d),y

    ; Read until first double quote.
l3: jsr read
    beq next_entry
    cmp #$22
    bne l3

    ; Read name till quote.
    ldy #dirent_name
l4: jsr read
    bcs error2
    beq next_entry
    cmp #$22
    beq n
    sta (d),y
    iny
    jmp l4
n:  lda #0
    sta (d),y

    ; Read till end of line.
l5: jsr read
    bne l5

    ; Step to next dirent and also save, so we can ignore the last line.
    lda d
    clc
    adc #dirent_size
    sta d
    bcc next_entry
    inc d+1
    jmp next_entry

error2:
    jsr READST
    bne done
    rts

done:
    lda #$02
    jsr CLOSE
    bcs e
    jsr CLRCHN
    lda d
    ldx d+1
    rts

e:  jmp error

read:
    jsr READST
    bne eof

    jsr CHRIN
    bcs e
    pha
    lda $90
    cmp #1   ; set carry when ST>0 (i.e., <>0!)
    pla      ; keep carry, and possibly set Z flag for byte=0
    rts

eof:
    clc
    rts
.endproc

; s: String                                                                                          
;
; Returns:
; Y: length, excluding terminating zero.
.proc strlen
    pha
    ldy #0
l:  lda (s),y
    beq n
    iny
    jmp l
n:  pla
    rts
.endproc