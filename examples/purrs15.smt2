; Cohen & Katcoff, Table II, p. 268, 6.
; parametric
; x(n) = 7 * x(n-1) + 10 * (2^(n-1) * c)^2
; Solution: x(n) = 7^n * x(0) + c^2 * 10/3 * (7^n - 4^n)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)
(declare-fun c () Int)

(assert (> n 0))
(assert (distinct
  (+ (* (exp 7 n) x0) (* c c (/ 10 3) (- (exp 7 n) (exp 4 n))))
  (+ (* 7 (+ (* (exp 7 (- n 1)) x0) (* c c (/ 10 3) (- (exp 7 (- n 1)) (exp 4 (- n 1)))))) (* 10 (exp 2 (- n 1)) (exp 2 (- n 1)) c c))
))

(check-sat)

