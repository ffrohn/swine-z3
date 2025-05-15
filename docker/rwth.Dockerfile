FROM z3-build AS swine-z3-build

RUN xbps-install -yS boost-devel cln-devel gmp-devel

RUN git clone git@github.com:ffrohn/swine-z3.git
WORKDIR /swine-z3
RUN mkdir /swine-z3/build
WORKDIR /swine-z3/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC=ON ..
RUN make -j
RUN mv swine-z3 /usr/bin/