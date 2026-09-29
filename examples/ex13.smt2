(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (or (> (* x y) 8) (< (* x y) (- 8))))
(assert (= (** (** x y) y) (** x (** y y))))

(check-sat)
