; parametric
; Here a != 0
; x(n) = a * x(n-1)
; Solution: x(n) = a^n * x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)
(declare-fun a () Int)

(assert (> n 0))
(assert (distinct a 0))
(assert (distinct
  (* (exp a n) x0)
  (* a (* (exp a (- n 1)) x0))
))

(check-sat)