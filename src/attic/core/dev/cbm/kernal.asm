;;; CBM KERNAL

;; SYSTEM CALLS
; Based on Simon Rowe's list at:
; http://sleepingelephant.com/ipw-web/bulletin/bb/viewtopic.php?f=2&t=7795&p=84292
ACPTR = $ffa5
CHKIN = $ffc6
CHKOUT = $ffc9
CHRIN = $ffcf
CHROUT = $ffd2
CIOUT = $ffa8
CLALL = $ffe7
CLOSE = $ffc3
CLRCHN = $ffcc
GETIN = $ffe4
IOBASE = $fff3
LISTEN = $ffb1
;LOAD = $ffd5
MEMBOT = $ff9c
MEMTOP = $ff99
OPEN = $ffc0
PLOT = $fff0
RDTIM = $ffde
READST = $ffb7
RESTOR = $ffba
SAVE = $ffd8
SCNKEY = $ff9f
;SCREEN = $ffed
SECOND = $ff93
SETLFS = $ffba
SETMSG = $ff90
SETNAM = $ffbd
SETTIM = $ffdb
SETTMO = $ffa2
CBM_STOP = $ffe1
TALK = $ffb4
TKSA = $ff96
UDTIM = $ffea
UNLSN = $ffae
UNTLK = $ffab
VECTOR = $ff8d

;; DEVICE NUMBERS
CBMDEV_KEYBOARD = 0
CBMDEV_DATASSETTE = 1
CBMDEV_RS232 = 2
CBMDEV_SCREEN = 3
CBMDEV_PRINTER0 = 4
CBMDEV_PRINTER1 = 5
CBMDEV_PLOTTER0 = 6
CBMDEV_PLOTTER1 = 7
CBMDEV_DRIVE8 = 8
