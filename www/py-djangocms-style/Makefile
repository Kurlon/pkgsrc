# $NetBSD: Makefile,v 1.1 2016/03/14 20:37:52 adam Exp $

DISTNAME=	djangocms-style-1.7.0
PKGNAME=	${PYPKGPREFIX}-${DISTNAME}
CATEGORIES=	www python
MASTER_SITES=	http://pypi.python.org/packages/source/d/djangocms-style/

MAINTAINER=	adam@NetBSD.org
HOMEPAGE=	https://github.com/divio/djangocms-style/
COMMENT=	Plugin for django CMS to add CSS classes to other plugins
LICENSE=	modified-bsd

USE_LANGUAGES=		# empty

DEPENDS+=	${PYPKGPREFIX}-django-cms-[0-9]*:../../www/py-django-cms

.include "../../lang/python/egg.mk"
.include "../../mk/bsd.pkg.mk"
