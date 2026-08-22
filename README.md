# NetBase
A base layer to easily create:
* Highly performant
* Highly available
* Highly scalable

TCP based protocols and networked applications upon.

NetBase makes use of Overlapped I/O and IOCP on Windows, and io_uring on Linux, to achieve high performance and scalability.
It also provides a simple and easy-to-use API for developers to create their own networked applications and protocols.

NetBase is not yet at 1.0 and thus does not have a stable API yet, however is current at the state where it can be used to develop real applications and protocols.

## TODO's
* Multiprocessed strategy to increase avaliability and performance.
* Async accepting of connections, pass off the socket to child processes.
* Transform main process into an application manager that spawns worker sub-processes.
* Add a configuration system instead of increasing the command line options.
* Stable C based API that doesnt make use of compiler specific ABI nonsense.
* Might remove the dependency to ASIO, its cool but I like writing this sorta stuff on my own.
* Remove the PimplPtr, was good for its time, but better off with just an hourglass approach for the API.
