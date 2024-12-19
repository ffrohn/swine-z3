; x(n) = 5 * x(n-1) - 6 * x(n-2) + 4
; Solution x(n) = x(0)*(3*2^n - 2*3^n) + x(1)*(3^n - 2^n) + 2*3^n - 4*2^n + 2

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+ (* x0 (- (* 3 (exp 2 n)) (* 2 (exp 3 n)))) (* x1 (- (exp 3 n) (exp 2 n))) (* 2 (exp 3 n)) (- (* 4 (exp 2 n))) 2)
  (+ (* 5
    (+ (* x0 (- (* 3 (exp 2 (- n 1))) (* 2 (exp 3 (- n 1))))) (* x1 (- (exp 3 (- n 1)) (exp 2 (- n 1)))) (* 2 (exp 3 (- n 1))) (- (* 4 (exp 2 (- n 1)))) 2)
  ) (- (* 6
    (+ (* x0 (- (* 3 (exp 2 (- n 2))) (* 2 (exp 3 (- n 2))))) (* x1 (- (exp 3 (- n 2)) (exp 2 (- n 2)))) (* 2 (exp 3 (- n 2))) (- (* 4 (exp 2 (- n 2)))) 2)
  )) 4)
))

(check-sat)