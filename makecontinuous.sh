#!/bin/bash

# Pre-calculate the hash string to reuse it for both builds
HASH_STAMP="$(echo "H$(cat .git/refs/heads/$2)" | cut -c1-7 | tr '[:lower:]' '[:upper:]')"

# Standard Build
make -j
printf "$HASH_STAMP" | dd of=build/jp/sm64.jp.z64 bs=1 seek=42 count=7 conv=notrunc
tools/sm64tools/n64cksum build/jp/sm64.jp.z64
make patch -j
cp build/jp/sm64.jp.bps "$1-$2.bps"

# HLE Build
make clean
make -j GRUCODE=f3d_20E
printf "$HASH_STAMP" | dd of=build/jp/sm64.jp.z64 bs=1 seek=42 count=7 conv=notrunc
tools/sm64tools/n64cksum build/jp/sm64.jp.z64
make patch -j GRUCODE=f3d_20E
cp build/jp/sm64.jp.bps "$1-$2-hle.bps"