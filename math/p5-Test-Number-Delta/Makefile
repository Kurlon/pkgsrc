# $NetBSD: Makefile,v 1.7 2015/06/12 10:50:31 wiz Exp $

DISTNAME=		Test-Number-Delta-1.06
PKGNAME=		p5-${DISTNAME}
PKGREVISION=		1
CATEGORIES=		math perl5
MASTER_SITES=		${MASTER_SITE_PERL_CPAN:=Test/}

MAINTAINER=		wen@NetBSD.org
HOMEPAGE=		http://search.cpan.org/dist/Test-Number-Delta
COMMENT=		Compare the difference between numbers against a given tolerance
LICENSE=		${PERL5_LICENSE}

PERL5_PACKLIST=		auto/Test/Number/Delta/.packlist

.include "../../lang/perl5/module.mk"
.include "../../mk/bsd.pkg.mk"
