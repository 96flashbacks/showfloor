make -j
printf "$(echo "#$(cat .git/refs/heads/$2)" | cut -c1-7 | tr '[:lower:]' '[:upper:]')" | dd of=build/jp/sm64.jp.z64 bs=1 seek=42 count=7 conv=notrunc
make patch -j
cp build/jp/sm64.jp.bps "$1-$2.bps"