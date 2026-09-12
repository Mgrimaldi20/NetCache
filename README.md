# NetCache
A networked TCP based cache/key-value store that is:
* Highly performant
* Highly available
* Highly scalable

NetCache makes use of Overlapped I/O and IOCP on Windows, and io_uring on Linux, to achieve high performance and scalability.

---
---

# NetCache Protocol v1

## Header

### Protocol Name
* **Type:** `uint8[2]`
* **Value:** ASCII `"NC"`

### Version
* **Type:** `uint8`
* **Value:** `0x01`

### Remaining Length
* **Type:** `uint32`
* **Description:** Remaining number of bytes following this field

---

## Command

| Command | Value | Description |
| :--- | :--- | :--- |
| **RES** | `0x00` | Reserved command, should not be used |
| **GET** | `0x01` | Request a cached value |
| **SET** | `0x02` | Store or update a cached value |
| **DEL** | `0x03` | Remove a key from the cache |

---

## GET Request

```text
[Command           : uint8  = 0x01]
[Key Length        : uint32]
[Key               : N bytes]
```

---

## SET Request

```text
[Command           : uint8  = 0x02]
[Key Length        : uint32]
[Key               : N bytes]
[Value Length      : uint32]
[Value             : N bytes]
```

---

## DEL Request

```text
[Command           : uint8  = 0x03]
[Key Length        : uint32]
[Key               : N bytes]
```

---

## Response

```text
[Status            : uint8]
[Remaining Length  : uint32]
[Payload           : Variable bytes]
```

---

## Status Codes

| Code | Status Name | Description |
| :--- | :--- | :--- |
| `0x00` | **OK** | The operation completed successfully. |
| `0x01` | **NOT_FOUND** | The requested key does not exist in the cache. |
| `0x02` | **INVALID_REQUEST** | The request contains invalid data or fields. |
| `0x03` | **INVALID_COMMAND** | The provided command byte is invalid. |
| `0x04` | **INTERNAL_ERROR** | An internal server error has occurred. |

---
---

## TODO's
* Introduce real multithreading combined with the async/await architecture, rather than just pure awaitables.
* Add a configuration system instead of increasing the command line options.
* Might remove the dependency to ASIO, its cool but I like writing this sorta stuff on my own.
