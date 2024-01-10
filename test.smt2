(declare-const x Int)
(declare-const y Int)
(declare-const A Bool)

(assert (edge (and (>= x 0) (< x 10)) (and (>= y 0) (< y 10))))
(assert (not (edge (and (>= x 0) (< x 10)) (> (+ x y) 10))))

(check-sat)
