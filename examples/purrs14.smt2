; Cohen & Katcoff, Table II, p. 268, 4.
; x(n) = x(n-2) + (n - 2)^2
; Solution: x(n) = 1/3 * n - 1/2 * n^2 + 1/6 * n^3 + 1/2 * (1 - (-1)^n) * x(1) + 1/2 * (1 + (-1)^n)*x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    (* (/ 1 3) n)
    (- (* (/ 1 2) n n))
    (* (/ 1 6) n n n)
    (* (/ 1 2) (- 1 (exp (- 1) n)) x1)
    (* (/ 1 2) (+ 1 (exp (- 1) n)) x0)
  )
  (+ (+ (* (/ 1 3) (- n 2)) (- (* (/ 1 2) (- n 2) (- n 2))) (* (/ 1 6) (- n 2) (- n 2) (- n 2)) (* (/ 1 2) (- 1 (exp (- 1) (- n 2))) x1) (* (/ 1 2) (+ 1 (exp (- 1) (- n 2))) x0)) (* (- n 2) (- n 2)))
))

(check-sat)

