(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1) v2) v2))
(assert (edge v3 true))
(assert (edge (or v0 (not v2)) (not v2)))
(assert (edge (or v0 (not v1) (not v2)) (not v4)))
(assert (edge (or v0 (not v2) (not v4)) (or v3 (not v4))))
(assert (not (edge v2 v1)))