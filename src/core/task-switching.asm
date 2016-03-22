; Calls over "take_over"s that haven't been "resume"d.
takeovers = $02a1

switch:
    ;;; Save process status.
    ; Save registers.
    sta saved_a
    stx saved_x
    sty saved_y
    pla
    sta saved_flags
    pla
    sta saved_pc
    pla
    sta @(++ saved_pc)
    tsx
    stx saved_sp

    jsr take_over

    ; Save stack.
l:  lda $100,x
    sta saved_stack,x
    inx
    bne -l

    ; Save zero page.
    ldx #$8f
l:  lda 0,x
    sta saved_zeropage,x
    dex
    bne -l
    lda 0
    sta saved_zeropage

    ; Save set of banks.
    lda $9ff6
    sta saved_bank_io
    lda $9ff8
    sta saved_bank1
    lda $9ffa
    sta saved_bank2
    lda $9ffc
    sta saved_bank3
    lda $9ffe
    sta saved_bank5
    ldy $9ff4
    ldx process_slot
    lda #0
    sta $9ff4
    tya
    sta process_cores_saved,x

switch_to_next_process:
    ; Switch to master core.
    lda #0
    sta $9ff4

    ; Get next process.
    ldx current_process
m:  inx
    cpx #max_num_processes
    bne +l
    ldx #0
l:  lda process_states,x
    bpl -m      ; Not running.

    ;;; Switch to found process.
    lda process_cores_saved,x

; Input:
; A: Core bank of process.
switch_to_process:
    ; Switch in process' core bank.
    sta $9ff4

    tay
    ldx process_slot
    lda #0
    sta $9ff4
    stx current_process
    sty $9ff4

    ; Restore stack contents.
    ldx saved_sp
    txs
l:  lda saved_stack,x
    sta $100,x
    inx
    bne -l

    ; Restore zero page.
    ldx #$8f
l:  lda saved_zeropage,x
    sta 0,x
    dex
    bne -l
    lda saved_zeropage
    sta 0

    lda saved_bank_io
    sta $9ff6
    lda saved_bank1
    sta $9ff8
    lda saved_bank2
    sta $9ffa
    lda saved_bank3
    sta $9ffc
    lda saved_bank5
    sta $9ffe

    lda @(++ saved_pc)
    pha
    lda saved_pc
    pha
    lda saved_flags
    pha
    lda saved_a
    ldx saved_x
    ldy saved_y

    jsr release
    rti

save_process_state:
    ; Save register contents.
    sta saved_a
    stx saved_x
    sty saved_y
    php
    pla
    sta saved_flags

    ; Save actually used core bank.
    ldy $9ff4
    ldx process_slot
    lda #0
    sta $9ff4
    sta process_cores_saved,x
    sty $9ff4

    ; Set return address to RTS that'll return from system call.
    lda #<+return
    sta saved_pc
    lda #>+return
    sta @(++ saved_pc)

    ; Save stack.
    tsx
    inx         ; Undo return address of this procedure.
    inx
    stx saved_sp
l:  lda $100,x
    sta saved_stack,x
    inx
    bne -l

    ; Save zero page.
    ldx #$9f
l:  lda 0,x
    sta saved_zeropage,x
    dex
    bpl -l

    ; Restore registers destroyed by this procedure except the flags.
    lda saved_a
    ldx saved_x

return:
    rts

take_over:
    pha
    lda #$7F
    sta $911e
    pla
    inc takeovers
    rts

release:
    php
    dec takeovers
    beq restart_task_switching
    plp
    rts

restart_task_switching:
    sei
    pha

    ; Disable NMI.
    lda #$7f
    sta $911e

    ; Enable Timer #1 on VIA 1 for NMI.
    lda #<switch
    sta $0318
    lda #>switch
    sta $0319   

    ; Load timer.
    lda #$00
    sta $9114
    lda #$80
    sta $9115

    ; Re–enable NMI.
    lda #$40        ; free–running
    sta $911b
    lda #$c0
    sta $911e

    pla
    plp
    rts
