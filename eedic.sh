#!/bin/sh

w3m -dump \
"http://kr.dictionary.search.yahoo.com/search/dictionaryp?p=$1&subtype=eng" \
2> /dev/null \
| awk 'BEGIN { flg = 0 } \
/단어 더 보기/ || /숙어/ || /예문/ { flg = 0 } \
{\
	if (flg) print $0 \
}/단어 \(영한\) { flg = 1 }' \
| sed 's/\[.*\]//g' | sed 's/?//g' | sed 's/단어장에 추가//g' | sed 's/단어장에//g' \
| sed -e 's/^추가//g' | sed -e 's/^[[:space:]]*)//g' | sed 's/<L//g' | sed -e 's/[□//g' \
| sed 's/()//g' | sed -e 's/^[[:space:]]*)//g' | sed 's/<L//g' | sed -e 's/[()]//g' \
| sed -e 's/[[:space:]]*$//g' | sed '/^$/d'
echo ""
