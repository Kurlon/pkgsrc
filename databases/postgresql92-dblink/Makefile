# $NetBSD: Makefile,v 1.7 2016/03/05 11:28:14 jperkin Exp $

PKGNAME=	${DISTNAME:C/-/92-dblink-/}
PKGREVISION=	1
COMMENT=	Dblink module for remote database connections

DEPENDS+=	postgresql92-server>=${PKGVERSION_NOREV}:../../databases/postgresql92-server

.include "../../databases/postgresql92/Makefile.common"

CONFIGURE_ARGS+=	--with-openssl
BUILD_MAKE_FLAGS=	PGXS=1

BUILD_DIRS=		contrib/dblink

.include "../../databases/postgresql92-client/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
