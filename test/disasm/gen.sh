#!/bin/sh
tmpfile=$(mktemp /tmp/gen-arm-script.XXXXXX)
echo "$tmpfile"
dd if=/dev/urandom of="$tmpfile" bs=1k count=1 \
 && arm-none-eabi-objdump -b binary -D -Mforce-thumb -Mreg-names-std -marm "$tmpfile" \
 | ./sanitize.sh
rm "$tmpfile"
