(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v0) (not v1)) (or (not v1) v2)))
(assert (edge (or v1 v3) (or v3 (not v4))))
(assert (edge (or (not v1) v2) (or v0 (not v1) (not v3))))
(assert (edge (or v1 v2 (not v0)) v0))
(assert (edge true v2))
(assert (not (edge v4 (or v2 v4))))