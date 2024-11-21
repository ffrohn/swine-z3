# SwInE -- SMT with Integer Exponentiation

SwInE is an SMT solver with support for integer exponentitaion.
In our corresponding [paper](10.1007/978-3-031-63498-7_21), the underlying approach is explained in detail.

It is based on "standard" SMT solvers without support for integer exponentiation.
In the [original implementation](https://github.com/ffrohn/swine), the underlying SMT solver can be exchanged via a command line flag.
This repository contains a reimplementation that directly uses the API of [Z3](https://github.com/Z3Prover/z3).
Hence, it is more efficient and robust than the original implementation, but the underlying solver is fixed.

# Usage

See ``swine-z3 --help``.

# Build (Linux only)

For the impatient: install [Docker Compose](https://docs.docker.com/compose/) and run ``scripts/build.sh``

To obtain a portable binary, we statically link against [musl-libc](https://www.musl-libc.org/).
As most Linux distributions are based on glibc instead, SwInE is built in a docker container, by default.
To this end, we provide two Dockerfiles:

* ``docker/z3.Dockerfile`` for building a statically linked version of Z3
* ``docker/Dockerfile`` for building a statically linked version of SwInE

Then ``docker/docker-compose.yml`` ensures that Z3 is built before SwInE, and that the container for building SwInE has access to the clone of the SwInE repository on your local file system.
Finally, ``scripts/build.sh`` invokes ``docker compose`` with the appropriate arguments to trigger the build.
If the build succeeds, a binary ``swine-z3`` and a static library ``libswine-z3.a`` will be available in the root directory of your clone of the SwInE repository, afterwards.

The script ``scripts/configure_and_build.sh`` is just invoked within the docker container for building SwInE, you should not invoke it directly.

# Development

The recommended way for developing SwInE is to use a [devcontainer](https://code.visualstudio.com/docs/devcontainers/containers).
To do so, execute ``scripts/extract_z3.sh`` once.
This script triggers a build of Z3, if necessary, and copies the libraries and header files that are needed to build SwInE to your local file system, so that they can be accessed from the devcontainer.

Afterwards, open a devcontainer in the root directory of your clone of the SwInE repository.
For example, for [VS Code](https://code.visualstudio.com/), this can be done by running ``code .``, pressing ``F1``, and selecting ``Reopen in devcontainer``.
