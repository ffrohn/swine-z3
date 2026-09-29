(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)

(assert (> (** 2 (+ x 1)) (** 2 x)))

(check-sat)
(get-model)
