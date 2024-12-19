; x(n) = 3 * x(n-1) + 2
; Solution: x(n) = 3^n * x(0) + 3^n - 1

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* (exp 3 n) x0) (exp 3 n) (- 1))
  (+ (* 3 (+ (* (exp 3 (- n 1)) x0) (exp 3 (- n 1)) (- 1))) 2)
))

(check-sat)