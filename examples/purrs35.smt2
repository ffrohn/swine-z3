; x(n) = x(n-2)
; Solution: x(n) = 1/2 * x(0) * (1 + (-1)^n) + 1/2 * x(1) * (1 - (-1)^n) 

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    (* (/ 1 2) x0 (+ 1 (exp (- 1) n)))
    (* (/ 1 2) x1 (- 1 (exp (- 1) n)))
  )
  (+
    (* (/ 1 2) x0 (+ 1 (exp (- 1) (- n 2))))
    (* (/ 1 2) x1 (- 1 (exp (- 1) (- n 2))))
  )
))

(check-sat)