; "Spanish" recurrence n. 2 
; x(n) = 2 * x(n-1) + 2^n
; Solution: x(n) = (x(0) + n) * 2^n

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
  (* (+ x n) (iexp 2 n))
  (+ (* 2 (* (+ x (- n 1)) (iexp 2 (- n 1)))) (iexp 2 n))
))

(check-sat)

