# $NetBSD: Makefile,v 1.16 2016/03/03 19:42:29 shattered Exp $

DISTNAME=	Config-Find-0.31
PKGNAME=	p5-${DISTNAME}
CATEGORIES=	devel perl5
MASTER_SITES=	${MASTER_SITE_PERL_CPAN:=Config/}

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://search.cpan.org/dist/Config-Find/
COMMENT=	Perl module for finding configuration files in the native OS fashion
LICENSE=	${PERL5_LICENSE}

DEPENDS+=	p5-File-Which>=0.05:../../devel/p5-File-Which
DEPENDS+=	p5-File-HomeDir>=0.80:../../devel/p5-File-HomeDir

PERL5_PACKLIST=		auto/Config/Find/.packlist

post-extract:
	cd ${WRKSRC} && chmod a-x lib/Config/Find.pm lib/Config/Find/*.pm

.include "../../lang/perl5/module.mk"
.include "../../mk/bsd.pkg.mk"
