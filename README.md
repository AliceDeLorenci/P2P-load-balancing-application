# SSC0904

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