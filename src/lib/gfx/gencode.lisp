(var *syscalls* nil)
(var *bytecodes* nil)

(fn syscallbyindex (x)
  (elt (+ *syscalls* *bytecodes*) x))

(fn syscall-name (x)
  (downcase (symbol-name x.)))

(fn syscall-bytecodes-source ()
  (apply #'+ (maptimes [format nil "c_~A=~A~%.export c_~A~%" (syscall-name (syscallbyindex _)) (++ _) (syscall-name (syscallbyindex _))]
                       (length (+ *syscalls* *bytecodes*)))))

(fn syscall-vectors (label prefix)
  (+ (format nil ".export ~A~%" label)
	 label ": "
     " .byte "
     (apply #'+ (pad (mapcar [format nil "~A~A" prefix (syscall-name _)]
                             (+ *syscalls* *bytecodes*))
                     ", "))
     (format nil "~%")))

(fn syscall-vectors-l () (syscall-vectors "syscall_vectors_l" "<"))
(fn syscall-vectors-h () (syscall-vectors "syscall_vectors_h" ">"))
(fn syscall-args-l () (syscall-vectors "syscall_args_l" "<args_"))
(fn syscall-args-h () (syscall-vectors "syscall_args_h" ">args_"))

(fn syscall-args ()
  (apply #'+ (mapcar [+ (format nil ".export args_~A~%" (syscall-name _))
					    (format nil "args_~A: .byte " (syscall-name _))
                        (apply #'+ (pad (+ (list (princ (length ._) nil))
									       (mapcar [downcase (symbol-name _)] ._))
										", "))
						(format nil "~%")]
                     (+ *syscalls* *bytecodes*))))

(fn syscall-imports ()
  (+ (format nil ".importzp d, tmp, tmp2, tmp3, ph, p, width, height, patternh, pattern, xpos, ypos~%")
	 (apply #'+ ".import "
		        (pad (mapcar [format nil "~A" (syscall-name _)]
                             (print (+ *syscalls* *bytecodes*)))
				     ", "))
	 (format nil "~%")))

(macro define-syscall (name &rest args)
  (| (assoc name *syscalls*)
     (acons! name args *syscalls*))
  nil)

(macro define-bytecode (name &rest args)
  (| (assoc name *bytecodes*)
     (acons! name args *bytecodes*))
  nil)

;(fn syscall-table ()
;  (mapcan [asm (format nil "jmp ~A" (syscall-name _))] *syscalls*))

;;;;;;;;;;;;;;;;;;;
;;; Moving data ;;;
;;;;;;;;;;;;;;;;;;;

(define-bytecode setzb tmp tmp2)
(define-bytecode setzw tmp tmp2 tmp3)
(define-bytecode setzs d tmp)

;;;;;;;;;;;;;;;;;;;
;;; Arithmetics ;;;
;;;;;;;;;;;;;;;;;;;

(define-bytecode addzb tmp tmp2 tmp3)
(define-bytecode sbczb tmp tmp2 tmp3)
;(define-bytecode addzbi tmp tmp2 tmp3)
(define-bytecode sbczbi tmp tmp2)
(define-bytecode addx tmp)
(define-bytecode addy tmp)

;;;;;;;;;;;;;
;;; Stack ;;;
;;;;;;;;;;;;;

(define-bytecode pushz tmp tmp2)
(define-bytecode popz tmp tmp2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Graphics primitives ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define-syscall calcscr xpos ypos)
(define-syscall setpattern pattern pattern+1)
(define-syscall vline xpos ypos height)
(define-syscall hline xpos ypos width)
(define-syscall frame xpos ypos width height)
(define-syscall box xpos ypos width height)
(define-syscall putstring p ph)
(define-syscall putchar)
(define-syscall get_text_width s s+1)

;;;;;;;;;;;;;;;;;;;;;;
;;; Function calls :::
;;;;;;;;;;;;;;;;;;;;;;
(define-bytecode apply)

(= *syscalls* (reverse *syscalls*))
(= *bytecodes* (reverse *bytecodes*))

(with-output-file o "src/lib/gfx/_bytecodes.asm"
  (princ (+ (syscall-imports)
            (format nil ".importzp s~%")
            (format nil ".data~%")
            (syscall-bytecodes-source)
            (syscall-vectors-l)
            (syscall-vectors-h)
            (syscall-args-l)
            (syscall-args-h)
            (syscall-args))
         o))

(with-output-file o "src/lib/gfx/_column-addrs.asm"
  (princ (+ (format nil ".export column_addrs_lo, column_addrs_hi~%.data~%")
            "column_addrs_lo: .byte "
            (apply #'+ (pad (maptimes [princ (low (* 16 12 _)) nil] 20) ", "))
            (format nil "~%")
            "column_addrs_hi: .byte "
            (apply #'+ (pad (maptimes [princ (high (* 16 12 _)) nil] 20) ", "))
            (format nil "~%"))
         o))

(quit)
