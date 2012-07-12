CC_MIPS="mips64-octeon-linux-gnu-gcc -mabi=n32"
CC=gcc
#rpcgen -I -s udp square.x -o square_svc.c
rpcgen square.x
#mips64-octeon-linux-gnu-gcc -mabi=n32 -g -o server server.c square_svc.c square_xdr.c
#mips64-octeon-linux-gnu-gcc -mabi=n32 -g -o client client.c square_clnt.c square_xdr.c
gcc -g -o square_server server.c square_svc.c square_xdr.c
gcc -g -o square_client client.c square_clnt.c square_xdr.c
