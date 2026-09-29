; x(n) = 4^(n-1)*x(n-1)+2^((n-1)^2)
; Solution: x(n) = -2*2^(n^2)*(1/4)^n+2^(1+n^2)*(1/2)^n+2^(n^2)*(1/2)^n*x(0) 

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)

(assert (> n 0))
(assert (distinct
  (+
    (* (- 2) (** 2 (* n n)) (/ 1 (** 4 n)))
    (* (** 2 (+ 1 (* n n))) (/ 1 (** 2 n)))
    (* (** 2 (* n n)) (/ 1 (** 2 n)) x0)
  )
  (+
    (* (** 4 (- n 1)) (+
      (* (- 2) (** 2 (* (- n 1) (- n 1))) (/ 1 (** 4 (- n 1))))
      (* (** 2 (+ 1 (* (- n 1) (- n 1)))) (/ 1 (** 2 (- n 1))))
      (* (** 2 (* (- n 1) (- n 1))) (/ 1 (** 2 (- n 1))) x0)
    ))
    (** 2 (* (- n 1) (- n 1)))
  )
))

(check-sat)