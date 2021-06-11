;coin21.ss
(define VARIANT 20)
(define LAST-DIGIT-OF-GROUP-NUMBER 8)
(define KINDS-OF-COINS 3)

(define (first-denomination kinds-of-coins)
 (cond((= kinds-of-coins 1) 1)
      (else (cond((= kinds-of-coins 2) 2)
                 (else (cond((= kinds-of-coins 3) 3)
                            (else 0)
                       );cond...
                 );else...
            );cond...
      );else...
 );cond...
)

(define (count-change amount)
 (display "______\namount: ")
 (display amount)
 (newline)
 (display "KINDS-OF-COINS: ")
 (display KINDS-OF-COINS)
 (newline)
 (let(
      (largest-coin (first-denomination KINDS-OF-COINS))
     )
     (display "largest-coin: ")
     (display largest-coin)
     (newline)
     (cond((> amount 0)
           (cond((> KINDS-OF-COINS 0)
                 (cond((> largest-coin 0)
                       (display "List of coin denominations: ")
                       (denomination-list KINDS-OF-COINS)
                       (display "count-change= ")
                       (cc amount KINDS-OF-COINS)
                      )
                      (else (display "Improper parametr value!\ncount-change= ")-1)                                      
                 );cond...
                )
                (else (display "Improper parametr value!\ncount-change= ")-1)
           );cond...
          )
          (else (display "Improper parametr value!\ncount-change= ")-1)
      );cond...
 )
)

(define (pier? x? y?)
 (= 1(cond((not x?)(cond((not y?) 1);if both x? and y? equal #f
                        (else 0);if y? equal #t
                   )
          )
          (else 0);if x? equal #t
      )
 )
)

(define (cc amount kinds-of-coins)
 (cond((= amount 0) 1)
      (else (cond((pier? (> 0 amount) (= kinds-of-coins 0))
                  (+ (cc amount (- kinds-of-coins 1))
                     (cc (- amount (first-denomination kinds-of-coins)) kinds-of-coins)))
                 (else 0)
            );cond...
      );else...
 );cond...
)


(define (denomination-list kinds-of-coins)
 (cond((= kinds-of-coins 0) (newline) 0)
       (else (display (first-denomination kinds-of-coins))
             (display " ")
             (denomination-list (- kinds-of-coins 1))
        )
 )
)

(define (GR-AMOUNT)
  (remainder (+ (* 100 LAST-DIGIT-OF-GROUP-NUMBER) VARIANT) 231)
)

(display "Variant ")
(display VARIANT)
(newline)
(newline)
(display (count-change 100)) (newline)
(display (count-change (GR-AMOUNT))) (newline)
(set! KINDS-OF-COINS 13)
(display (count-change 100)) (newline)
(display"(c) Marinichev I.A. 2021\n")
