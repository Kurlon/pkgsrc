# $NetBSD: Makefile,v 1.5 2016/03/05 11:27:44 jperkin Exp $
#

DISTNAME=	grantlee-0.5.1
PKGREVISION=	2
CATEGORIES=	devel
MASTER_SITES=	http://downloads.grantlee.org/

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://www.gitorious.org/grantlee/pages/Home
COMMENT=	String template engine in QT based on Django template system
LICENSE=	gnu-lgpl-v3

USE_LANGUAGES=  c c++
USE_CMAKE=      yes

pre-configure:
	${LN} -sf ${QTDIR}/bin/qmake ${BUILDLINK_DIR}/bin/

.include "../../x11/qt4-libs/buildlink3.mk"
.include "../../x11/qt4-tools/buildlink3.mk"

.include "../../mk/bsd.pkg.mk"
