; sat: y = 0, x = 1
(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (= x (exp 2 y)))

(push)

(assert (>= y 0))

(check-sat)
(get-model)

(pop)

(check-sat)
(get-model)
