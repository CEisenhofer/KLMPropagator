(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (not v0) (or (not v1) (not v0) (not v2))))
(assert (edge (not v3) (or (not v1) v2 v3)))
(assert (edge (not v3) true))
(assert (edge (or v1 v3 v4) (or v0 (not v3))))
(assert (edge v2 v4))
(assert (not (edge true (or (not v0) (not v2)))))