# $NetBSD: Makefile,v 1.32 2015/12/05 21:25:58 adam Exp $

DISTNAME=	duplicity-0.6.26
CATEGORIES=	sysutils net
MASTER_SITES=	http://code.launchpad.net/duplicity/0.6-series/${PKGVERSION_NOREV}/+download/

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://www.nongnu.org/duplicity/
COMMENT=	Remote encrypting incremental backup utility
LICENSE=	gnu-gpl-v2

USE_PKGLOCALEDIR=	yes

PKG_OPTIONS_VAR=	PKG_OPTIONS.duplicity
PKG_SUPPORTED_OPTIONS=	gnupg2
DEPENDS+=	${PYPKGPREFIX}-lockfile>=0.9.1:../../devel/py-lockfile
.include "../../mk/bsd.options.mk"
.if !empty(PKG_OPTIONS:Mgnupg2)
DEPENDS+=	gnupg2-[0-9]*:../../security/gnupg2
GPGPATH=	${LOCALBASE}/bin/gpg2
.else
DEPENDS+=	gnupg>=1.0.0:../../security/gnupg
GPGPATH=	${LOCALBASE}/bin/gpg
.endif

REPLACE_PYTHON+=	bin/*
PYTHON_VERSIONS_INCOMPATIBLE=	33 34 35 # not yet ported as of 0.6.22

SUBST_CLASSES+=			fix-paths
SUBST_STAGE.fix-paths=		pre-configure
SUBST_MESSAGE.fix-paths=	fixing hard coded dirs
SUBST_FILES.fix-paths=		setup.py
SUBST_SED.fix-paths=		-e 's,share/man/man1,${PKGMANDIR}/man1,'
SUBST_SED.fix-paths+=		-e 's,share/locale,${PKGLOCALEDIR}/locale,'

SUBST_CLASSES+=			gpgpath
SUBST_STAGE.gpgpath=		pre-configure
SUBST_FILES.gpgpath=		duplicity/gpginterface.py
SUBST_VARS.gpgpath=		GPGPATH

# XXX in 0.6.21, doc files are mode 0600
#post-extract:
#	cd ${WRKSRC} && ${CHMOD} go+r *

.include "../../lang/python/application.mk"
.include "../../lang/python/egg.mk"
.include "../../net/librsync/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
