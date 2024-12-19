; x(n) = 5 * x(n-1) - 6 * x(n-2) + 4 + 5^n
; Solution: x(n) = 2 + 25/6*5^n + 13/2*2^n - 21/2*3^n + (3*2^n-2*3^n)*x(0) + (-2^n+3^n)*x(1)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    2
    (* (/ 25 6) (exp 5 n))
    (* (/ 13 2) (exp 2 n))
    (- (* (/ 21 2) (exp 3 n)))
    (* (- (* 3 (exp 2 n)) (* 2 (exp 3 n))) x0)
    (* (- (exp 3 n) (exp 2 n)) x1)
  )
  (+
    (* 5 (+
      2
      (* (/ 25 6) (exp 5 (- n 1)))
      (* (/ 13 2) (exp 2 (- n 1)))
      (- (* (/ 21 2) (exp 3 (- n 1))))
      (* (- (* 3 (exp 2 (- n 1))) (* 2 (exp 3 (- n 1)))) x0)
      (* (- (exp 3 (- n 1)) (exp 2 (- n 1))) x1)
    ))
    (- (* 6 (+
      2
      (* (/ 25 6) (exp 5 (- n 2)))
      (* (/ 13 2) (exp 2 (- n 2)))
      (- (* (/ 21 2) (exp 3 (- n 2))))
      (* (- (* 3 (exp 2 (- n 2))) (* 2 (exp 3 (- n 2)))) x0)
      (* (- (exp 3 (- n 2)) (exp 2 (- n 2))) x1)
    )))
    4
    (exp 5 n)
  )
))

(check-sat)