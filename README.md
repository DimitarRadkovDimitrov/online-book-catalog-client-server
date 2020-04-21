# Online Book Catalog

Client and server programs implementing the [online book catalog protocol](./RFC/cis3210_ass1_RFC32108_dimitar.pdf). Clients communicate with book catalog server over TCP sockets. Operations include submit, get, and remove.

<br>

## Build 

* Build in the root directory using makefile.
    ```
    make
    ```

<br>

## Run

* Start the server, binding it to a port. 
    ```
    ./server <port_number>
    ```

* Start one or more clients and connect them to the server port.
    ```
    ./client <server_address> <port_number>
    ```

<br>

## Known Limitations
* Server cannot accept a single newline as input.
* Maximum size for client request and server response is 1024 characters.

<br>

## References
* With permission, used socket client and server code given by Dave 
  OHallaron at Carnegie Mellon University.
