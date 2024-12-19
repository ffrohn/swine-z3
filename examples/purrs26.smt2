; x(n) = 2 * x(n-1) + 1
; Solution: x(n) = 2^n * x(0) + 2^n - 1

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* (exp 2 n) x0) (exp 2 n) (- 1))
  (+ (* 2 (+ (* (exp 2 (- n 1)) x0) (exp 2 (- n 1)) (- 1))) 1)
))

(check-sat)