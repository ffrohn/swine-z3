; x(n) = 2^n * x(n-1)
; Solution: x(n) = 2^(1/2*n+1/2*n^2) * x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(declare-fun iexp (Int Int) Int)
(assert (forall ((b Int)) (= (iexp b 0) 1)))
(assert (forall ((b Int) (e Int)) (=> (> e 0) (= (iexp b e) (* b (iexp b (- e 1)))))))
(assert (forall ((b Int) (e Int)) (=> (< e 0) (= (iexp b e) (iexp b (- e))))))

(assert (> n 0))
(assert (distinct
  (* (iexp 2 (+ (* (/ 1 2) n) (* (/ 1 2) n n))) x0)
  (* (iexp 2 n) (* (iexp 2 (+ (* (/ 1 2) (- n 1)) (* (/ 1 2) (- n 1) (- n 1)))) x0))
))

(check-sat)