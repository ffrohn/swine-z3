#!/bin/bash

# triggers a swine build in a docker container

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
USER="$(id -u):$(id -g)"
docker compose --file $SCRIPT_DIR/../docker/docker-compose.yml build
docker compose --file $SCRIPT_DIR/../docker/docker-compose.yml up
