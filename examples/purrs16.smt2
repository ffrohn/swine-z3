; Cohen & Katcoff, Table II, p. 268, 7.
; parametric
; x(n) = (1-a) * x(n-1) + b * (1 - x(n-1))
; Solution: x(n) = (1 - a - b)^n * x(0) - (a + b)^(-1) * b * (1 - a - b)^n + b * (a + b)^(-1)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)
(declare-fun a () Int)
(declare-fun b () Int)

(assert (> n 0))
(assert (distinct a (- b)))
(assert (distinct
  (+ (* (exp (+ 1 (- a) (- b)) n) x0) (- (* (/ 1 (+ a b)) b (exp (+ 1 (- a) (- b)) n))) (* b (/ 1 (+ a b))))
  (+ (* (- 1 a)
    (+ (* (exp (+ 1 (- a) (- b)) (- n 1)) x0) (- (* (/ 1 (+ a b)) b (exp (+ 1 (- a) (- b)) (- n 1)))) (* b (/ 1 (+ a b))))
  ) (* b (- 1
    (+ (* (exp (+ 1 (- a) (- b)) (- n 1)) x0) (- (* (/ 1 (+ a b)) b (exp (+ 1 (- a) (- b)) (- n 1)))) (* b (/ 1 (+ a b))))
  )))
))

(check-sat)

