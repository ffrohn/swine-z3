; x(n) = 2 * x(n-1) + 3^n
; Solution: x(n) = 3 * (3^n - 2^n) + 2^n * x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+ (* 3 (- (** 3 n) (** 2 n))) (* (** 2 n) x))
  (+ (* 2 (+ (* 3 (- (** 3 (- n 1)) (** 2 (- n 1)))) (* (** 2 (- n 1)) x))) (** 3 n))
))

(check-sat)

