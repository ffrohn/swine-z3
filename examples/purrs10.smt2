; x(n) = 1/2 * x(n-1) + 1/2 * x(n-2) + 1
; Solution: x(n) = 1/3 * (1 + 2 * pow(-1/2, n)) * x(0) + 2/3 * (1 - pow(-1/2, n)) * x(1) + 2/3 * n - 4/9 + 4/9 * pow(-1/2, n)

(set-logic ALL)
(set-option :produce-models true)
(declare-fun x0 () Int)
(declare-fun x1 () Int)
(declare-fun n () Int)

(assert (> n 1))
(assert (distinct
  (+
    (* (/ 1 3) (+ 1 (* 2 (/ (exp (- 1) n) (exp 2 n)))) x0)
    (* (/ 2 3) (- 1 (/ (exp (- 1) n) (exp 2 n))) x1)
    (* (/ 2 3) n)
    (- (/ 4 9))
    (* (/ 4 9) (/ (exp (- 1) n) (exp 2 n)))
  )
  (+
    (* (/ 1 2)
      (+
        (* (/ 1 3) (+ 1 (* 2 (/ (exp (- 1) (- n 1)) (exp 2 (- n 1))))) x0)
        (* (/ 2 3) (- 1 (/ (exp (- 1) (- n 1)) (exp 2 (- n 1)))) x1)
        (* (/ 2 3) (- n 1))
        (- (/ 4 9))
        (* (/ 4 9) (/ (exp (- 1) (- n 1)) (exp 2 (- n 1))))
      )
    )
    (* (/ 1 2)
      (+
        (* (/ 1 3) (+ 1 (* 2 (/ (exp (- 1) (- n 2)) (exp 2 (- n 2))))) x0)
        (* (/ 2 3) (- 1 (/ (exp (- 1) (- n 2)) (exp 2 (- n 2)))) x1)
        (* (/ 2 3) (- n 2))
        (- (/ 4 9))
        (* (/ 4 9) (/ (exp (- 1) (- n 2)) (exp 2 (- n 2))))
      )
    )
    1
  )
))

(check-sat)

