# $NetBSD: Makefile,v 1.10 2015/07/29 10:30:51 wiz Exp $

DISTNAME=	4.02+6
PKGNAME=	camlp4-${DISTNAME:S/+/./}
PKGREVISION=	1
CATEGORIES=	lang
MASTER_SITES=	https://github.com/ocaml/camlp4/archive/
DIST_SUBDIR=	ocaml-camlp4

MAINTAINER=	jaapb@NetBSD.org
HOMEPAGE=	http://github.com/ocaml/camlp4/
COMMENT=	Preprocessor and pretty printer for OCaml
LICENSE=	gnu-lgpl-v2

HAS_CONFIGURE=	yes
WRKSRC=		${WRKDIR}/camlp4-4.02-6
MAKE_JOBS_SAFE=	no

.include "../../mk/ocaml.mk"

BUILD_TARGET=	byte camlp4/META
.if ${OCAML_USE_OPT_COMPILER} == "yes"
BUILD_TARGET+=	native
.endif
INSTALLATION_DIRS+=	${OCAML_SITELIBDIR}/camlp4

do-install:
	cd ${WRKSRC} && BINDIR="${DESTDIR}${PREFIX}/bin" \
		LIBDIR="${DESTDIR}${PREFIX}/lib/ocaml" ${SH} build/install.sh
	${INSTALL_DATA} ${WRKSRC}/camlp4/META ${DESTDIR}${PREFIX}/${OCAML_SITELIBDIR}/camlp4

.include "../../mk/bsd.pkg.mk"
