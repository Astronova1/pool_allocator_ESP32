# pool_allocator_ESP32

# Real-Time IoT Telemetry Backend & Network Pipeline

A lightweight, multi-threaded C++ backend server integrated with an ESP32 microcontroller running custom C++ firmware. 

Instead of relying on slow text-based formats like JSON or HTTP protocols, this project streams packed, raw binary memory payloads directly across TCP sockets using POSIX network interfaces and Windows Sockets (Winsock2).

---

## 🚀 Key Features

* **Low-Latency Binary Serialization:** Transfers telemetry data using packed binary memory blocks (`TelemetryPacket` structures) directly across raw byte streams, completely bypassing string-parsing overhead.
* **Embedded C++ Firmware:** Programmed an ESP32 microcontroller using the **lwIP network stack**, featuring automatic WiFi reconnect loops and non-blocking socket state checks.
* **Core Network Primitives:** Built server socket lifecycles manually utilizing POSIX and Winsock2 APIs, managing socket handles, non-blocking address resolution (`getaddrinfo`), port binding, listening, and client connections.
* **Data Integrity & Network Resiliency:** Implemented strict `SOCKET_ERROR` handling, dynamic byte buffering, and graceful teardown semantics (`closesocket`, `freeaddrinfo`).

---

## 🛠 Tech Stack

* **Server:** C++, Winsock2 API, POSIX Sockets, TCP/IP Protocol Stack
* **Microcontroller / Firmware:** ESP32, C++, lwIP Stack, Arduino Framework (Wokwi Simulation Environment)
* **Peripherals:** DHT22 Temperature & Humidity Sensor

---

## 📡 Architecture & Memory Layout

### 1. Telemetry Data Structure
Data is packed into an unpadded 20-byte binary layout to minimize network bandwidth consumption:

```cpp
struct TelemetryPacket {
    uint32_t client_id;  // Unique client hardware identifier
    uint32_t packet_id;  // Sequence counter tracking dropped packets
    float position_x;    // Coordinate payload / Sensor parameter A (Temperature)
    float position_y;    // Coordinate payload / Sensor parameter B (Humidity)
    float sensor_value;  // Secondary telemetry reading
};


## Compile with 
```bash
cl server.cpp /EHsc /Fe:server.exe ws2_32.lib
```
