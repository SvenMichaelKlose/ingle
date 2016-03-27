FILE_OPENED     = 128
FILE_STREAM     = 64    ; Otherwise block–oriented.
FILE_READABLE   = 1
FILE_WRITABLE   = 2

    data

    org $0400

0 0 0       ; JMP to link().


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Information in master core 0. ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This area is not used in other banks than #0.

core_data_start = @*pc*

;;; Processes

current_process:        0   ; Index into following tables.
process_states:         fill max_num_processes
process_cores:          fill max_num_processes
process_cores_saved:    fill max_num_processes


;;; Banks allocated by "alloc_block":
master_banks:       fill @(/ 1024 8)


;;; Virtual files which can be shared by processes.
vfile_states:       fill max_num_vfiles ; Like in file_states.
vfile_drivers:      fill max_num_vfiles
vfile_handles:      fill max_num_vfiles ; Within drivers.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Per–process information in each copy of the core. ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

per_process_data_size = @(- per_process_data_end per_process_data_start)
per_process_data_start:

;;; Task state.
; +3K area is stored in process_cores_saved in the master core.
saved_pc:           0 0
saved_a:            0
saved_x:            0
saved_y:            0
saved_flags:        0
saved_sp:           0
saved_stack:        fill 256
saved_zeropage:     fill $90    ; BASIC part only.
saved_bank_io:      0
saved_bank1:        0
saved_bank2:        0
saved_bank3:        0
saved_bank5:        0


;;; Banks allocated via "load".
bank_ram:   0
bank_io:    0
bank1:      0
bank2:      0
bank3:      0
bank5:      0

parent_process: 0
program_start:  0 0
process_slot:   0       ; Indexes in master core tables.


;;; File system

;; States of opened files.
file_states:    fill max_num_files_per_process

;; Indexes into virtual file nodes which may be shared amongst processes.
file_nodes:     fill max_num_files_per_process


;;; Banks allocated by "alloc_block":
banks:      fill @(/ 1024 8)


;;; Libraries
num_libraries:      0
libraries:          fill max_num_libraries_per_process

end_of_library_calls: 0 0
library_calls:

per_process_data_end:

    end
