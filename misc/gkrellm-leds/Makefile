# $NetBSD: Makefile,v 1.43 2016/03/05 11:28:54 jperkin Exp $
#

DISTNAME=	gkleds-0.8.2
PKGNAME=	${DISTNAME:S/^gk/gkrellm-/}
PKGREVISION=	29
CATEGORIES=	misc
MASTER_SITES=	http://heim.ifi.uio.no/~oyvinha/files/files/

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://heim.ifi.uio.no/~oyvinha/gkleds/
COMMENT=	GKrellM plugin that monitors CapsLock, NumLock and ScrollLock keys

USE_TOOLS+=	pkg-config
GNU_CONFIGURE=	YES
USE_LIBTOOL=	YES

GNU_CONFIGURE_LIBSUBDIR=gkrellm2/plugins

.include "../../sysutils/gkrellm/buildlink3.mk"
.include "../../x11/libXtst/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
