(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (not v0) (or v1 (not v0) (not v2))))
(assert (edge (or v1 v3 v4) (or (not v1) v2)))
(assert (edge (or v0 v4 (not v1)) (not v2)))
(assert (edge (or v0 v2) true))
(assert (edge (or v1 (not v2)) (or v0 (not v1))))
(assert (not (edge v3 (or (not v3) (not v4) (not v0)))))