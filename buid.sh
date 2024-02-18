#!/usr/bin/env bash
docker pull ubuntu
docker build -f Dockerfile.env -t ubuntu-cpp .
docker build --no-cache -f Dockerfile.build -t 705-udp-server .