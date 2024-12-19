; Cohen & Katcoff, Table II, p. 268, 3.
; x(n) = -2 * x(n-1) - x(n-2) + 2^n
; Solution: x(n) = 4/9 * 2^n + (-1)^n * n * 4/3 - (-1)^n * 4/9 + (-1)^n *(1-n) * x(0) - (-1)^n * n * x(1)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    (* (/ 4 9) (exp 2 n))
    (* (exp (- 1) n) n (/ 4 3))
    (- (* (exp (- 1) n) (/ 4 9)))
    (* (exp (- 1) n) (- 1 n) x0)
    (* (exp (- 1) n) n x1)
  )
  (+ (* (- 2) (+
    (* (/ 4 9) (exp 2 (- n 1)))
    (* (exp (- 1) (- n 1)) (- n 1) (/ 4 3))
    (- (* (exp (- 1) (- n 1)) (/ 4 9)))
    (* (exp (- 1) (- n 1)) (- 1 (- n 1)) x0)
    (* (exp (- 1) (- n 1)) (- n 1) x1)
  )) (- (+
    (* (/ 4 9) (exp 2 (- n 2)))
    (* (exp (- 1) (- n 2)) (- n 2) (/ 4 3))
    (- (* (exp (- 1) (- n 2)) (/ 4 9)))
    (* (exp (- 1) (- n 2)) (- 1 (- n 2)) x0)
    (* (exp (- 1) (- n 2)) (- n 2) x1)
  )) (exp 2 n))
))

(check-sat)

