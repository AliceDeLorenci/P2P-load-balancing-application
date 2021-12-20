# P2P Load Balacing Application

Through this load balancing application, peers can run their executable files on idle machines belonging to other peers. The peer that needs to run an executable elsewhere is called *sender*, as it will send its executable over the network; on the other hand, the peer that runs another's executable is called *receiver*.

This application employs a hybrid peer-to-peer architecture, in which a mediator server acts as an access point to the P2P network and mediates the connections by matching *senders* to *receivers*.

# Launching the application

Compile the **mediator** program with:

    make DEFS=-DMEDIATOR EXEC=mediator ODIR=build_mediator

and launch:

    ./mediator

Compile the **peer** program with:

    make DEFS=-DPEER EXEC=peer ODIR=build_peer

and launch:
    
    ./peer

The mediator must be up before any peers are launched as peers will attempt to contact the mediator.

# Testing the application

The ```test``` executable can be used to test the application. It outputs "Hello word!" every second, 10 times.

---

Course:
- Distributed Computer Systems (SSC0904)

Professor:
- Dr. Jo Ueyama

Group:
- Alice Valença De Lorenci 			   						
- Guilherme Ramon Rodrigues da Silva 							
- Henrique Martins Fernandes 		  							
- Rodolfo Cola Genaro 			 

Universidade de São Paulo (USP), 2021.
