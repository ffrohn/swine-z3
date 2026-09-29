; x(n) = 2 * x(n-1) + 4^(n-1)
; Solution: x(n) = 2^n * x(0) - 2^(n-1) + 1/2 * 4^n

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    (* (** 2 n) x0)
    (- (** 2 (- n 1)))
    (* (/ 1 2) (** 4 n))
  )
  (+
    (* 2
      (+
        (* (** 2 (- n 1)) x0)
        (- (** 2 (- n 2)))
        (* (/ 1 2) (** 4 (- n 1)))
      )
    )
    (** 4 (- n 1))
  )
))

(check-sat)