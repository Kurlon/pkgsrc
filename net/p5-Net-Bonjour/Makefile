# $NetBSD: Makefile,v 1.9 2015/06/12 10:50:45 wiz Exp $
#

DISTNAME=		Net-Bonjour-0.96
PKGNAME=		p5-${DISTNAME}
PKGREVISION=		6
CATEGORIES=		net
MASTER_SITES=		${MASTER_SITE_PERL_CPAN:=Net/}

MAINTAINER=		pkgsrc-users@NetBSD.org
HOMEPAGE=		http://search.cpan.org/dist/Net-Bonjour/
COMMENT=		Module for DNS service discovery (Apple's Bonjour/Rendezvous)
LICENSE=		${PERL5_LICENSE}

DEPENDS+=		p5-Net-DNS>=0.50:../../net/p5-Net-DNS

SUPERSEDES+=		p5-Net-Rendezvous-[0-9]*

PERL5_PACKLIST=		auto/Net/Bonjour/.packlist

.include "../../lang/perl5/module.mk"
.include "../../mk/bsd.pkg.mk"
