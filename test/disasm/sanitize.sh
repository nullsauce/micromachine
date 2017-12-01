#!/bin/sh
sed 's/\t/ /g'         `# tabs to space` \
| sed '/ \.\.\./d'        `# remove inter file spacers` \
| sed 's/ ; (.*//'     `# remove line suffixes` \
| sed 's/ ; 0x.*//'    `# remove line suffixes` \
| sed '/\.w/d'         `# remove wide (.w) instructions` \
| sed -e s/'  ; '//g   `# cleanup <unsupported> mentions` \
| sed '/stmdb/d'       `# remove stmdb instructions` \
| sed '/ldc/d'         `# remove coprocessor instructions` \
| sed '/stc/d'         `# remove coprocessor instructions` \
| sed '/hlt/d'         `# remove halt instructions` \
| sed '/cbnz/d'        `# remove cbnz instructions` \
| sed '/cbz/d'         `# remove cbz instructions` \
| sed '/ rsb/d'        `# remove rsb instructions` \
| sed '/ mrrc/d'       `# remove rsb instructions` \
| sed '/subw/d'        `# remove subw instructions` \
| sed '/ vmls/d'       `# remove vmls instructions` \
| sed '/ ldrd/d'       `# remove ldrd instructions` \
| sed 's/ aif/ i/'     `# fix cpsid aif -> cpsid i`