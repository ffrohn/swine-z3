; "Spanish" recurrence n. 1 
; x(n) = 2 * x(n-1) + 2^(n-1) + n + 1
; Solution: x(n) = x(0) * 2^n + n * 2^(n-1) + 3 * 2^n - n - 3

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* x (exp 2 n)) (* n (exp 2 (- n 1))) (* 3 (exp 2 n)) (- n) (- 3))
  (+ (* 2 (+ (* x (exp 2 (- n 1))) (* (- n 1) (exp 2 (- n 2))) (* 3 (exp 2 (- n 1))) (- 1 n) (- 3))) (exp 2 (- n 1)) n 1)
))

(check-sat)

