; x(n) = 2 * x(n-1) + 3^n
; Solution: x(n) = 3 * (3^n - 2^n) + 2^n * x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)

(declare-fun iexp (Int Int) Int)
(assert (forall ((b Int)) (= (iexp b 0) 1)))
(assert (forall ((b Int) (e Int)) (=> (> e 0) (= (iexp b e) (* b (iexp b (- e 1)))))))
(assert (forall ((b Int) (e Int)) (=> (< e 0) (= (iexp b e) (* b (iexp b (+ e 1)))))))

(assert (> n 0))
(assert (distinct
  (+ (* 3 (- (iexp 3 n) (iexp 2 n))) (* (iexp 2 n) x))
  (+ (* 2 (+ (* 3 (- (iexp 3 (- n 1)) (iexp 2 (- n 1)))) (* (iexp 2 (- n 1)) x))) (iexp 3 n))
))

(check-sat)

