![Nightly Build](https://github.com/ffrohn/swine-z3/actions/workflows/build-all.yml/badge.svg)
![Linux Build](https://github.com/ffrohn/swine-z3/actions/workflows/build-linux.yml/badge.svg)
![Static Linux Build](https://github.com/ffrohn/swine-z3/actions/workflows/build-linux-static.yml/badge.svg)
![macOS Build](https://github.com/ffrohn/swine-z3/actions/workflows/build-osx.yml/badge.svg)
![Windows Build](https://github.com/ffrohn/swine-z3/actions/workflows/build-windows.yml/badge.svg)

# SwInE -- SMT with Integer Exponentiation

SwInE is an SMT solver with support for integer exponentitaion.
In our corresponding [paper](10.1007/978-3-031-63498-7_21), the underlying approach is explained in detail.

It is based on "standard" SMT solvers without support for integer exponentiation.
In the [original implementation](https://github.com/ffrohn/swine), the underlying SMT solver can be exchanged via a command line flag.
This repository contains a reimplementation that directly uses the API of [Z3](https://github.com/Z3Prover/z3).
Hence, it is more efficient and robust than the original implementation, but the underlying solver is fixed.

# Usage

See ``swine-z3 --help``.

# Build

Please check our [workflow files](https://github.com/ffrohn/swine-z3/tree/master/.github/workflows) to see how to build SwInE on all major platforms.

# Development

The recommended way for developing SwInE is to use a [devcontainer](https://code.visualstudio.com/docs/devcontainers/containers).
To do so, execute ``scripts/extract_z3.sh`` once.
This script triggers a build of Z3, if necessary, and copies the libraries and header files that are needed to build SwInE to your local file system, so that they can be accessed from the devcontainer.

Afterwards, open a devcontainer in the root directory of your clone of the SwInE repository.
For example, for [VS Code](https://code.visualstudio.com/), this can be done by running ``code .``, pressing ``F1``, and selecting ``Reopen in devcontainer``.
