#!/bin/sh

w3m -dump \
"http://kr.dictionary.search.yahoo.com/search/dictionaryp?p=$1&subtype=eng" \
2> /dev/null \
| awk 'BEGIN { flg = 0 } \
/�ܾ� �� ����/ || /����/ || /����/ { flg = 0 } \
{\
	if (flg) print $0 \
}/�ܾ� \(����\) { flg = 1 }' \
| sed 's/\[.*\]//g' | sed 's/?//g' | sed 's/�ܾ��忡 �߰�//g' | sed 's/�ܾ��忡//g' \
| sed -e 's/^�߰�//g' | sed -e 's/^[[:space:]]*)//g' | sed 's/<L//g' | sed -e 's/[��//g' \
| sed 's/()//g' | sed -e 's/^[[:space:]]*)//g' | sed 's/<L//g' | sed -e 's/[()]//g' \
| sed -e 's/[[:space:]]*$//g' | sed '/^$/d'
echo ""
