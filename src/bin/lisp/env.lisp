;(? (== nil nil) ;(== 49 49)
;(print (quote error)))
(print 1)
(+ 1 1)
(- 1 1)
(* 23 42)
(/ 23 5)
(% 23 5)
(++ 2)
(-- 2)
(quote x)
(fn myfun (x)
  (print x))
(fn fnord (x))
myfun
;(== 49 (myfun 49))
;(? (not (== 49 (myfun 49)))
;   (print (quote error)))
(? t 1 2)
(? nil 1 2)
(gc)
myfun
(myfun 128)
(var some-list (quote (1 2 3 4)))
(fn length (x)
  (? (cons? x)
     (+ 1 (length (cdr x)))
     0))
(length some-list)
(gc)
ok