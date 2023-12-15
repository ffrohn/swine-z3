FROM voidlinux/voidlinux-musl:latest as base

ENV CFLAGS -march=x86-64 -O2
ENV CXXFLAGS $CFLAGS
RUN echo "repository=https://repo-default.voidlinux.org/current/musl" > /etc/xbps.d/00-repository-main.conf
RUN xbps-install -yS xbps
RUN xbps-install -yS gcc git automake autoconf make cmake wget python-devel bash



FROM base as z3

RUN xbps-install -yS python3-devel

RUN wget https://github.com/Z3Prover/z3/archive/refs/tags/z3-4.12.2.tar.gz
RUN tar xf z3-4.12.2.tar.gz
RUN mv /z3-z3-4.12.2 /z3
WORKDIR /z3
RUN mkdir build
WORKDIR /z3/build
RUN cmake -DZ3_BUILD_LIBZ3_SHARED=FALSE -DCMAKE_BUILD_TYPE=Release ..
RUN make -j
RUN make install



FROM voidlinux/voidlinux-musl:latest as swine-z3-docker

RUN mkdir /my_include/
RUN mkdir /my_lib/

COPY --from=z3 /z3/src /z3/src
COPY --from=z3 /usr/local/lib64/libz3.a /my_lib
COPY --from=z3 /usr/local/include/z3*.h /my_include
