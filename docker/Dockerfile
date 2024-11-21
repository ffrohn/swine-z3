FROM z3-build AS swine-z3-build

RUN xbps-install -yS boost-devel cln-devel gmp-devel

CMD ["/swine-z3/scripts/configure_and_build.sh"]
