A multiuser server and client for SOS (basically tick tack toe with a configurable-sized board and 
points).

The interesting constraint for this project was that the client and server must use the same class
to interface with the network. All message sending and receiving code must be in the Session class, 
which also handles message passing (in the code sense) to alert the server or client to the event of
recieving a message (network sense).

Note to Dr. Kreahling: I changed the names of a few things. MultiuserSos* is renamed Sos*, since 
we're only doing a multiuser version (and not a single user one), it's redundant. Given the length 
of "Multiuser", it saves a lot of typing to get rid of it. NetworkInterface is renamed Session, 
because that better describes the functionality of the class, IMHO.
