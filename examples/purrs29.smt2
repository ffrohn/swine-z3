; x(n) = x(n-1) + 1 / (n*(n+1)) + 5
; Solution: x(n) = (1+n)^(-1)*n + x(0) + 5*n

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* (exp (+ 1 n) (- 1)) n) x0 (* 5 n))
  (+ (+ (* (exp n (- 1)) (- n 1)) x0 (* 5 (- n 1))) (/ 1 (* n (+ n 1))) 5)
))

(check-sat)