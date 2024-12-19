; x(n) = x(n-1) + 1 / (n*(n+1)) + 5
; Solution: x(n) = (1+n)^(-1)*n + x(0) + 5*n

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
  (+ (* (iexp (+ 1 n) (- 1)) n) x0 (* 5 n))
  (+ (+ (* (iexp n (- 1)) (- n 1)) x0 (* 5 (- n 1))) (/ 1 (* n (+ n 1))) 5)
))

(check-sat)