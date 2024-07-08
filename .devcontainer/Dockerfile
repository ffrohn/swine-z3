FROM mcr.microsoft.com/devcontainers/base:alpine-3.19

ENV CFLAGS -march=x86-64
ENV CXXFLAGS $CFLAGS
ENV LD_LIBRARY_PATH=/workspaces/swine-z3/docker/base-image/usr/local/lib
ENV CPATH=/workspaces/swine-z3/docker/base-image/usr/local/include

RUN apk add cmake boost-dev gdb
