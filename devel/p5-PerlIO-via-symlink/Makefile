# $NetBSD: Makefile,v 1.18 2015/06/12 10:49:39 wiz Exp $

DISTNAME=	PerlIO-via-symlink-0.05
PKGNAME=	p5-${DISTNAME}
PKGREVISION=	7
CATEGORIES=	devel perl5
MASTER_SITES=	${MASTER_SITE_PERL_CPAN:=OurNet/CLKAO/}

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://search.cpan.org/~clkao/PerlIO-via-symlink/
COMMENT=	PerlIO layers to create symlinks
LICENSE=	${PERL5_LICENSE}

PERL5_PACKLIST=		auto/PerlIO/via/symlink/.packlist
PERL5_MODULE_TYPE=	Module::Install

.include "../../lang/perl5/module.mk"
.include "../../mk/bsd.pkg.mk"
