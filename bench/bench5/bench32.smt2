(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 v1) (or v3 (not v2))))
(assert (edge (not v3) (or v0 v1 (not v2))))
(assert (edge (or v4 (not v0)) v4))
(assert (edge v1 true))
(assert (edge v4 (not v1)))
(assert (not (edge true v4)))