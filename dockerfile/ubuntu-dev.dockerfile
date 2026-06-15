FROM ubuntu:latest

RUN apt update && apt install -y \
    build-essential \
    gcc \
    g++ \
    make \
    git \
    python3 \
    curl \
    vim \
    net-tools \
    bridge-utils
