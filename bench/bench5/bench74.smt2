(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v0) (not v1) (not v2)) (or v3 (not v1))))
(assert (edge true (or v1 (not v4))))
(assert (edge (or (not v1) v2) (or v1 (not v2))))
(assert (edge (or (not v0) (not v2)) (or (not v1) (not v3))))
(assert (edge (or (not v0) (not v1)) v1))
(assert (not (edge (or v1 v4) (or v1 (not v3) (not v0)))))