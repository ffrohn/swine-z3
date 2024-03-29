# Triggers a build within a docker container. Assumes that the LoAT root
# directory is mounted at /LoAT.

FROM voidlinux/voidlinux-musl:latest as base

RUN echo "repository=https://repo-default.voidlinux.org/current/musl" > /etc/xbps.d/00-repository-main.conf
RUN xbps-install -yS xbps
RUN xbps-install -yS gcc git make cmake
RUN xbps-install -yS boost-devel cln-devel gmp-devel
RUN xbps-install -yS bash

RUN git config --global --add safe.directory /swine-z3

FROM base as build

COPY --from=swine-z3-docker:latest /my_lib/ /usr/local/lib
COPY --from=swine-z3-docker:latest /my_include/ /usr/local/include

CMD ["/swine-z3/scripts/configure_and_build.sh"]

