(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1)) (or v0 v2)))
(assert (edge true (or (not v1) v3 (not v2))))
(assert (edge (or (not v0) (not v3)) (not v2)))
(assert (edge (not v4) true))
(assert (edge (not v4) (or (not v1) (not v4) (not v0))))
(assert (not (edge (not v1) (or v1 (not v0)))))