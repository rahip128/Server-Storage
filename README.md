# Server-Storage

This program writes a server for a simple key-value storage service. Client programs may use this server to
set and retrieve the values associated with keys, where the keys and values are strings. For example,
one client may set the key “day” to the value “Sunday”. If a second client then requested the value
of “day”, the server would reply “Sunday”.

Keys and values may be strings of any length, but may not include the terminator (\0) or newline
(\n).

The server program opens and binds a listening socket on the specified port and waits for
incoming connection requests. For each connection, it creates a thread to handle communication
with that client. The thread terminates once the connection is closed.
The server also maintains the key-value data in a data structure shared by all threads. A linked list was used and maintained in alphabetical order whilst making sure to synchronize access to the structure to avoid non-deterministic behavior

Clients initiate and terminate connections to the server. Once a connection is open, a client will
send zero or more requests to the server. For each request, the server will take some action and
respond. After receiving a response, the client may send another request or close the connection.

Our communication protocol has three requests that the client may send to the server.

GET key: Requests the current value associated with key. If key is set, the server returns its value.
Otherwise it returns key-not-found error.

SET key: Associates value with key. 

DEL key: Deletes key from storage. If key is set, it returns the value that was associated with it
and unsets the key. Otherwise, it returns key-not-found error.

Example:
The client opens a connection to the server and sends this message, setting the value of “day” to
“Sunday”.

SET
11
day
Sunday

Note that each field in the message ends with a newline character.
The server sets “day” and reports success.
OKS
