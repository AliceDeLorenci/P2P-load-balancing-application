# SSC0904

## TO DO

- "Hello World" each 0.5s
- Append socket number to executable copy name and output file name

- ler e enviar o executável por blocos para evitar estouro de heap
- ```htonl()```, ```ntohl()```
- once the child process is up, it must be killed before the parent quits
- receiver can give up
- maximum queue size, to many receivers or senders: drop
- mediator-receiver connection could be maintained and receiver could listen to it with a thread, if the mediator decides to drop the receiver

### Mediator

Accept sender:
- if exits receiver:
    - send receiver socket
    - pop receiver
- if not exists receiver:
    - push sender

Accept receiver:
- if exists sender:
    - send receiver socket
    - pop sender
- if not exists sender:
    - push receiver

Functionalities not includede in the prototype:
- receiver informs for how long it expects to be up
- sender informs how long it beleives it will take
- mediator matches sender and receiver


## P2P

- https://github.com/mitrapinaki/PeerToPeer
- https://github.com/taylorconor/p2psc
- https://github.com/um4ng-tiw/Peer-to-Peer-Socket-C

## Execução

Compilar com:

    make DEFS=-DMEDIATOR EXEC=mediator ODIR=build_mediator

    make DEFS=-DPEER EXEC=peer ODIR=build_peer

Executar:

    ./server
    ./client

https://www.ibm.com/docs/en/zos/2.3.0?topic=directives-conditional-compilation


- conceito / embasamento
- provavelmente a aplicação rodaria dentro de um container por segurança
- peer to peer
- mandar executável

