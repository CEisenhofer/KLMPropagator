(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v1 (not v0)) (not v2)))
(assert (edge (or v0 (not v2)) (or v0 v3 (not v1))))
(assert (edge (or v1 (not v3) v4) (or v1 (not v3))))
(assert (edge (or v1 v2) (or v1 v4)))
(assert (edge (or v1 v3 (not v4)) (or v1 (not v3) (not v4))))
(assert (not (edge (not v0) (or (not v3) (not v4)))))