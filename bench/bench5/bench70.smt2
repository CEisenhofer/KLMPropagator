(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1)) (or v2 (not v1))))
(assert (edge (or v3 (not v2)) (or (not v4) (not v0))))
(assert (edge true (or (not v4) (not v0))))
(assert (edge v1 (or v3 (not v1))))
(assert (edge true v1))
(assert (not (edge (or v0 v4) (or v4 (not v1) (not v3)))))