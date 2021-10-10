.export ingle_exec, init_proc, alloc_proc, free_proc

.import save_state
.import launch
.import alloc_bank, free_bank
.import set_banks, restore_banks

.bss

NUM_PROCESSES = 32

procs:          .res NUM_PROCESSES
proc_ram:       .res NUM_PROCESSES
proc_ram123:    .res NUM_PROCESSES
proc_io23:      .res NUM_PROCESSES
proc_blk1:      .res NUM_PROCESSES
proc_blk2:      .res NUM_PROCESSES
proc_blk3:      .res NUM_PROCESSES
proc_blk5:      .res NUM_PROCESSES

current_proc:   .res 1

.code

.proc init_proc
    ldx #NUM_PROCESSES
    lda #0
    sta current_proc
l1: sta procs,x
    dex
    bpl l1

    rts
.endproc

; Return new process in X.
.proc alloc_proc
    lda $9ff4
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    ldx #0
l1: lda procs,x
    beq allocate_banks
    inx
    cpx #NUM_PROCESSES
    bne l1
error:
    sec
    bcs return      ; (jmp)

allocate_banks:
    lda #1
    sta procs,x
    jsr alloc_bank
    bcs error
    sta proc_ram,x
    jsr alloc_bank
    bcs error
    sta proc_ram123,x
    jsr alloc_bank
    bcs error
    sta proc_io23,x
    jsr alloc_bank
    bcs error
    sta proc_blk1,x
    jsr alloc_bank
    bcs error
    sta proc_blk2,x
    jsr alloc_bank
    bcs error
    sta proc_blk3,x
    jsr alloc_bank
    bcs error
    sta proc_blk5,x

    clc
return:
    jmp restore_banks
.endproc

.proc ingle_exec
    lda $9ff4       ; Save RAM1,2,3.
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    jsr alloc_proc
    bcs return
    stx current_proc

    ; Configure banks for launch().
    lda proc_ram123,x
    ldy #0
    sta $0124
    sty $0125
    lda proc_io23,x
    sta $0126
    sty $0127
    lda proc_blk1,x
    sta $0128
    sty $0129
    lda proc_blk2,x
    sta $012a
    sty $012b
    lda proc_blk3,x
    sta $012c
    sty $012d
    lda proc_blk5,x
    sta $012e
    sty $012f

    jmp launch

return:
    jmp restore_banks
.endproc

; Free process X.
.proc free_proc
    lda $9ff4
    pha
    lda $9ff5
    pha
    lda $9ff1
    pha
    jsr set_banks

    lda procs,x
    bne ok
error:
    sec
    jmp restore_banks

ok: lda #0
    sta procs,x
    lda proc_ram,x
    jsr free_bank
    lda proc_ram123,x
    jsr free_bank
    lda proc_io23,x
    jsr free_bank
    lda proc_blk1,x
    jsr free_bank
    lda proc_blk2,x
    jsr free_bank
    lda proc_blk3,x
    jsr free_bank
    lda proc_blk5,x
    jsr free_bank

    clc
    jmp restore_banks
.endproc
