(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v1) v2 (not v0)) (or v0 (not v1))))
(assert (edge (or v3 v4) true))
(assert (edge (not v2) (or v2 v4)))
(assert (edge (or (not v1) v2 (not v0)) (not v0)))
(assert (edge true (or (not v0) (not v3))))
(assert (not (edge (or v4 (not v2)) (or v0 v4))))