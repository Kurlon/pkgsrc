# $NetBSD: buildlink3.mk,v 1.12 2015/08/07 11:19:13 jaapb Exp $
#

BUILDLINK_TREE+=	camlp5

.if !defined(CAMLP5_BUILDLINK3_MK)
CAMLP5_BUILDLINK3_MK:=

BUILDLINK_API_DEPENDS.camlp5+=	camlp5>=5.01
BUILDLINK_ABI_DEPENDS.camlp5+=	camlp5>=6.14
BUILDLINK_PKGSRCDIR.camlp5?=	../../lang/camlp5

.include "../../lang/ocaml/buildlink3.mk"
.endif # CAMLP5_BUILDLINK3_MK

BUILDLINK_TREE+=	-camlp5
