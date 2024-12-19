; Canonical decomposition of the "Spanish" recurrence n. 1, part 2
; x(n) = 2 * x(n-1) + 2^(n-1)
; Solution: x(n) = x(0) * 2^n + n * 2^(n-1)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* x0 (exp 2 n)) (* n (exp 2 (- n 1))))
  (+ (* 2 (+ (* x0 (exp 2 (- n 1))) (* (- n 1) (exp 2 (- (- n 1) 1))))) (exp 2 (- n 1)))
))

(check-sat)