; Symbolic, with double root (a != 0)
; parametric
; x(n) = 2 * a * x(n-1) - a^2 * x(n-2)
; Solution: x(n) = x(0) * (1 - n) * a^n + x(1) * n * a^n

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)
(declare-fun a () Int)

(assert (> a 0))
(assert (> n 1))
(assert (distinct
  (+ (* x0 (- 1 n) (exp a n)) (* x1 n (exp a n)))
  (- (* 2 a (+ (* x0 (- 1 (- n 1)) (exp a (- n 1))) (* x1 (- n 1) (exp a (- n 1))))) (* a a (+ (* x0 (- 1 (- n 2)) (exp a (- n 2))) (* x1 (- n 2) (exp a (- n 2))))))
))

(check-sat)

