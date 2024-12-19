; x(n) = 3 * x(n-1) + 2^n
; Solution: x(n) = 2 * (3^n - 2^n) + 3^n * x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* 2 (- (exp 3 n) (exp 2 n))) (* (exp 3 n) x))
  (+ (* 3 (+ (* 2 (- (exp 3 (- n 1)) (exp 2 (- n 1)))) (* (exp 3 (- n 1)) x))) (exp 2 n))
))

(check-sat)

