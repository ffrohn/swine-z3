; sat: x = 1, y = 0
(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (>= x 0))
(assert (>= y 0))
(assert (> (** 2 x) (** 3 y)))

(check-sat)
(get-model)
