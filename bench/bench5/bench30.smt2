(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v0) (not v1)) (or (not v1) (not v2))))
(assert (edge (or (not v0) (not v3)) (or v2 (not v3))))
(assert (edge (not v1) (or v1 (not v3) (not v0))))
(assert (edge (or v3 (not v4)) (or v0 (not v1) (not v3))))
(assert (edge (or v1 v3 (not v2)) (or (not v0) (not v2))))
(assert (not (edge (or (not v1) (not v3) v4) (or v4 (not v2)))))