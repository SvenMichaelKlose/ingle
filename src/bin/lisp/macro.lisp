(var *macros* nil)

(fn macro? (s)
  (find s *macros*))

(fn macroexpand-unquote (x)
  x)

(fn macroexpand (x)
  (? (cons? x)
     (?
       (eq (car x) 'quote)
         x
       (eq (car x) 'backquote)
         (macroexpand-unquote x)
       (@ macroexpand 
          (? (macro? (car x))
             (macroexpand (apply (car x) (cdr x)))
             x)))
     x))

(fn progn body
  $(block t
     ,@body))

(fn let (n v . body)
  (((g)
     $(((,n)
         ,@body
       ,v)))
   (symbol)))

(fn prog1 body
  (with (g (symbol))
    $(((,g)
        ,@(cdr body)
        ,g)
      ,(car body))))

(fn when (x . body)
  $(? ,x
      (progn
        ,@body)))

(fn unless (x . body)
  $(? (not ,x)
      (progn
        ,@body)))

(fn while (cond result . body)
  (let tag (symbol)
    $(block nil
       (or ,cond
           (return))
       ,@body)))

(fn dolist (iter . body)
  (with (v    (car iter)
         i    (cadr iter)
         r    (cddr iter)
         tag  (symbol))
    $(let ,v ,i
       (block nil
         (or ,v (return ,(car r)))
         ,tag
         ,body
         (= ,v (cdr v))
         (go ,tag)))))
