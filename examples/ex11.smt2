(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (> (abs x) 2))
(assert (> (abs y) 2))
(assert (= (exp (exp x y) y) (exp x (exp y y))))

(check-sat)
