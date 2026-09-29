; like purrs03, but a is fixed to 2

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)
(declare-fun a () Int)

(assert (> n 0))
(assert (= a 2))
(assert (distinct
  (+
    (* x (** a n))
    (div
      (+
        (** a (+ n 2))
        (** a (+ n 1))
        (- (* n n))
        (* (+ (* 2 n n) (* 2 n) (- 1)) a)
        (- (* (+ n 1) (+ n 1) a a))
      )
      (* (- a 1) (- a 1) (- a 1))
    )
  )
  (+ (* a (+ (* x (** a (- n 1))) (div (+ (** a (+ n 1)) (** a n) (- (* (- n 1) (- n 1))) (* (+ (* 2 (- n 1) (- n 1)) (* 2 (- n 1)) (- 1)) a) (- (* n n a a))) (* (- a 1) (- a 1) (- a 1))))) (* n n))
))

(check-sat)

