# $NetBSD: Makefile,v 1.9 2015/06/12 10:50:26 wiz Exp $

PKGNAME=	sendmail-qtool-${DIST_VERS}
PKGREVISION=	1
COMMENT=	Perl script to manage sendmail queue entries

PKG_OPTIONS=	#hack to please Makefile.common

.include "../../mail/sendmail/Makefile.common"

USE_TOOLS+=	pax
USE_TOOLS+=	perl:run

REPLACE_PERL=		contrib/qtool.pl

INSTALLATION_DIRS=	sbin  ${PKGMANDIR}/man8

do-build:

do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/contrib/qtool.pl \
		${DESTDIR}${PREFIX}/sbin/sendmail-qtool
	${INSTALL_MAN} ${WRKSRC}/contrib/qtool.8 \
		${DESTDIR}${PREFIX}/${PKGMANDIR}/man8/sendmail-qtool.8

.include "../../mk/bsd.pkg.mk"
