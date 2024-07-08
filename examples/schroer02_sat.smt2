(set-logic ALL)
(declare-fun init_c!10 () Int)
(declare-fun c_0!12 () Int)
(declare-fun slice_toggle_3!15 () Bool)
(declare-fun slice_toggle_4!16 () Bool)
(declare-fun slice_toggle_1!13 () Bool)
(declare-fun slice_toggle_0!11 () Bool)
(assert (>= init_c!10 0))
(assert (>= c_0!12 0))
(assert (let ((a!1 (ite (<= c_0!12 0)
                (not slice_toggle_4!16)
                (or (not slice_toggle_1!13) (not slice_toggle_3!15))))
      (a!2 (/ 1.0 (exp 3 (+ 1 (ite (>= c_0!12 1) (+ (- 1) c_0!12) 0)))))
      (a!7 (+ 1.0 (* (- 1.0) (/ 1.0 (exp 3 (+ 1 init_c!10)))))))
(let ((a!3 (ite slice_toggle_3!15
                (ite (>= 1.0 a!2) (+ 1.0 (* (- 1.0) a!2)) 0.0)
                1.0))
      (a!8 (ite (>= 1.0 (/ 1.0 (exp 3 (+ 1 init_c!10)))) a!7 0.0)))
(let ((a!4 (ite (<= c_0!12 0)
                (ite slice_toggle_4!16 0.0 1.0)
                (+ (/ 2.0 3.0) (* (/ 1.0 3.0) a!3)))))
(let ((a!5 (<= (* (- 1.0) (/ 1.0 (exp 3 (+ 1 c_0!12)))) (+ (- 1.0) a!4))))
(let ((a!6 (ite (>= 1.0 (/ 1.0 (exp 3 (+ 1 c_0!12)))) a!5 (<= 0.0 a!4))))
(let ((a!9 (and slice_toggle_0!11
                (<= (ite slice_toggle_0!11 a!8 1.0) (ite (or a!1 a!6) 1.0 0.0)))))
(let ((a!10 (<= a!8
                (ite (or a!1 a!6 a!9)
                     (ite slice_toggle_0!11 a!8 1.0)
                     (ite (or a!1 a!6) 1.0 0.0)))))
  (not (or (ite (or a!1 a!6 a!9) (not slice_toggle_0!11) (or a!1 a!6)) a!10))))))))))

(check-sat)
(get-model)