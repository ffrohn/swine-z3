; here a != 1 and a != 0
; parametric
; x(n) = a * x(n-1) + n^2
; Solution: x(n) = x(0) * a^n + ( a^(n+2) + a^(n+1) - n^2 + (2 * n^2 + 2 * n - 1) * a - (n + 1)^2 * a^2 ) / (a-1)^3 

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)
(declare-fun a () Int)

(assert (> n 0))
(assert (> a 1))
(assert (distinct
  (+
    (* x (exp a n))
    (div
      (+
        (exp a (+ n 2))
        (exp a (+ n 1))
        (- (* n n))
        (* (+ (* 2 n n) (* 2 n) (- 1)) a)
        (- (* (+ n 1) (+ n 1) a a))
      )
      (* (- a 1) (- a 1) (- a 1))
    )
  )
  (+ (* a (+ (* x (exp a (- n 1))) (div (+ (exp a (+ n 1)) (exp a n) (- (* (- n 1) (- n 1))) (* (+ (* 2 (- n 1) (- n 1)) (* 2 (- n 1)) (- 1)) a) (- (* n n a a))) (* (- a 1) (- a 1) (- a 1))))) (* n n))
))

(check-sat)

