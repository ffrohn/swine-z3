; sat: x = 1, y = 0
(set-logic ALL)
(set-option :produce-models true)
(declare-fun a () Int)
(declare-fun b () Int)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (< 1 a b))
(assert (> x 0))
;(assert (> y 0))
(assert (< (exp a x) (exp b x)))

(check-sat)
(get-model)
