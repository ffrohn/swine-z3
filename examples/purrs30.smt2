; parametric
; x(n) = x(n-1) + 1 / (n*(n+1)*(n+2))+a^n
; Solution: x(n) = -1+(1-a)^(-1)+(8+4*n^2+12*n)^(-1)*n^2+3*(8+4*n^2+12*n)^(-1)*n-(1-a)^(-1)*a^(1+n)+x(0)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun n () Int)
(declare-fun a () Int)

(assert (distinct a 1))
(assert (> n 0))
(assert (distinct
  (+
    (- 1)
    (/ 1 (- 1 a))
    (*
      (/ 1 (+ 8 (* 4 n n) (* 12 n)))
      (* n n)
    )
    (*
      3
      (/ 1 (+ 8 (* 4 n n) (* 12 n)))
      n
    )
    (- (*
      (/ 1 (- 1 a))
      (exp a (+ 1 n))
    ))
    x0
  )
  (+ (+
    (- 1)
    (/ 1 (- 1 a))
    (*
      (/ 1 (+ 8 (* 4 (- n 1) (- n 1)) (* 12 (- n 1))))
      (* (- n 1) (- n 1))
    )
    (*
      3
      (/ 1 (+ 8 (* 4 (- n 1) (- n 1)) (* 12 (- n 1))))
      (- n 1)
    )
    (- (*
      (/ 1 (- 1 a))
      (exp a (+ 1 (- n 1)))
    ))
    x0
  ) (/ 1 (* n (+ n 1) (+ n 2))) (exp a n))
))

(check-sat)