# $NetBSD: Makefile,v 1.35 2016/03/05 11:29:34 jperkin Exp $

PKGNAME=		${APACHE_PKG_PREFIX}-${PHP_PKG_PREFIX}-${PHP_VERSION}
PKGREVISION=		1
COMMENT=		Apache (${PKG_APACHE}) module for ${PKG_PHP}

CONFLICTS=		ap-php-[0-9]*

USE_TOOLS+=		lex pkg-config

APACHE_MODULE=		YES

.include "../../lang/php/phpversion.mk"
.include "${PHPPKGSRCDIR}/Makefile.php"

MESSAGE_SUBST+=		PKG_PHP_MAJOR_VERS=${PKG_PHP_MAJOR_VERS}

# Ensure we export symbols in the linked shared object.
CONFIGURE_ENV+=		EXTRA_LDFLAGS=${EXPORT_SYMBOLS_LDFLAGS:M*:Q}

INSTALLATION_DIRS=	lib/httpd

do-install:
	if [ -f ${WRKSRC}/.libs/libphp${PKG_PHP_MAJOR_VERS}.so ]; then	\
		${INSTALL_DATA} ${WRKSRC}/.libs/libphp${PKG_PHP_MAJOR_VERS}.so \
			${DESTDIR}${PREFIX}/lib/httpd/mod_php${PKG_PHP_MAJOR_VERS}.so;   \
	elif [ -f ${WRKSRC}/libs/libphp${PKG_PHP_MAJOR_VERS}.so ]; then	\
		${INSTALL_DATA} ${WRKSRC}/libs/libphp${PKG_PHP_MAJOR_VERS}.so  \
			${DESTDIR}${PREFIX}/lib/httpd/mod_php${PKG_PHP_MAJOR_VERS}.so;   \
	fi

.include "${PHPPKGSRCDIR}/buildlink3.mk"
.include "../../mk/apache.mk"
.include "../../mk/bsd.pkg.mk"
