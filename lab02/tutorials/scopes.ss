; scopes
(define a 7)
(define(f f); f - процедура и параметр
  (set! f (- f (/ 100 101)))
  (let(
       (a (sin f))
       (b (log f))
       (f 0); f - локальная переменая
      )
      (set! f (* 5 a b))
    f
  )
)
(f 1)
