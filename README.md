# NetCache
A networked TCP based cache/key-value store that is:
* Highly performant
* Highly available
* Highly scalable

NetCache makes use of Overlapped I/O and IOCP on Windows, and io_uring on Linux, to achieve high performance and scalability.

## TODO's
* Introduce real multithreading combined with the async/await architecture, rather than just pure awaitables.
* Add a configuration system instead of increasing the command line options.
* Might remove the dependency to ASIO, its cool but I like writing this sorta stuff on my own.
