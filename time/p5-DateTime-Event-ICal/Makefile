# $NetBSD: Makefile,v 1.16 2016/03/05 08:33:10 wen Exp $

DISTNAME=		DateTime-Event-ICal-0.13
PKGNAME=		p5-${DISTNAME}
CATEGORIES=		time perl5
MASTER_SITES=		${MASTER_SITE_PERL_CPAN:=DateTime/}

MAINTAINER=		pkgsrc-users@NetBSD.org
HOMEPAGE=		http://search.cpan.org/dist/DateTime-Event-ICal
COMMENT=		DateTime extension for computing rfc2445 recurrences
LICENSE=		${PERL5_LICENSE}

DEPENDS+=		p5-DateTime>=0.36:../../time/p5-DateTime
DEPENDS+=		p5-DateTime-Event-Recurrence>=0.16:../../time/p5-DateTime-Event-Recurrence

PERL5_PACKLIST=		auto/DateTime/Event/ICal/.packlist

.include "../../lang/perl5/module.mk"
.include "../../mk/bsd.pkg.mk"
