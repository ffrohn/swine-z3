(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun y () Int)

(assert (or (> x 2) (< x (- 2))))
(assert (or (> y 2) (< y (- 2))))
(assert (= (** (** x y) y) (** x (** y y))))

(check-sat)
