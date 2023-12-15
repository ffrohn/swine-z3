# Starts QtCreator within a docker container. Assumes that the LoAT root
# directory is mounted at /LoAT.

FROM voidlinux/voidlinux-musl:latest as base

RUN echo noextract=/etc/hosts > /etc/xbps.d/test.conf
RUN echo "repository=https://repo-default.voidlinux.org/current/musl" > /etc/xbps.d/00-repository-main.conf
RUN xbps-install -ySu xbps
RUN xbps-install -ySu
RUN xbps-install -yS gcc git make cmake bash
RUN xbps-install -yS boost-devel cln-devel gmp-devel
RUN xbps-install -yS qtcreator xauth mesa gdb clang-tools-extra

RUN git config --global --add safe.directory /swine-z3

FROM base as qtcreator

COPY --from=swine-z3-docker:latest /z3/src /z3/src
COPY --from=swine-z3-docker:latest /my_lib/ /usr/local/lib
COPY --from=swine-z3-docker:latest /my_include/ /usr/local/include

ARG TOKEN
RUN xauth add $TOKEN

CMD ["/usr/bin/qtcreator", "-settingspath", "/swine-z3/", "-notour", "/swine-z3/CMakeLists.txt"]
