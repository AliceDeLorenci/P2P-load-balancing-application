# SSC0904

## TO DO

- substituir ```system()``` por ```fork()```
- ler e enviar o executável por blocos para evitar estouro de heap
- ```htonl()```, ```ntohl()```
- once the child process is up, it must be killed before the parent quits

## P2P

- https://github.com/mitrapinaki/PeerToPeer
- https://github.com/taylorconor/p2psc
- https://github.com/um4ng-tiw/Peer-to-Peer-Socket-C

## Execução

Compilar com:

    make DEFS=-DSERVER EXEC=server ODIR=build_server

    make DEFS=-DCLIENT EXEC=client ODIR=build_client

Executar:

    ./server
    ./client

https://www.ibm.com/docs/en/zos/2.3.0?topic=directives-conditional-compilation


- conceito / embasamento
- provavelmente a aplicação rodaria dentro de um container por segurança
- peer to peer
- mandar executável

