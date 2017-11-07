#!/bin/sh
tmpfile=$(mktemp /tmp/gen-arm-script.XXXXXX)
echo "$tmpfile"
dd if=/dev/urandom of="$tmpfile" bs=1k count=1 \
 && arm-none-eabi-objdump -b binary -D -Mforce-thumb -Mreg-names-std -marm "$tmpfile" \
 | sed 's/\t/ /g'       `# tabs to space` \
 | sed 's/ ; (.*//'     `# remove line suffixes` \
 | sed 's/ ; 0x.*//'    `# remove line suffixes` \
 | sed '/\.w/d'         `# remove wide (.w) instructions` \
 | sed -e s/'  ; '//g   `# cleanup <unsupported> mentions` \
 | sed '/stmdb/d'       `# remove stmdb instructions` \
 | sed '/ldc/d'         `# remove coprocessor instructions` \
 | sed '/stc/d'         `# remove coprocessor instructions` \
 | sed '/hlt/d'         `# remove halt instructions` \
 | sed '/cbnz/d         `# remove cbnz instructions` \
 | sed '/cbz/d'         `# remove cbz instructions` \
 | sed '/ rsb/d'        `# remove rsb instructions` \
 | sed '/ mrrc/d'       `# remove rsb instructions` \
 | sed '/subw/d'        `# remove subw instructions` \
 | sed '/ vmls/d'        `# remove vmls instructions` \
 | sed '/ ldrd/d'        `# remove ldrd instructions` \
 | sed 's/ aif/ i/'      `# fix cpsid aif -> cpsid i`
rm "$tmpfile"
