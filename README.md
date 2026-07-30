# Pool_allocator_ESP32

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

### 1. Payload Serialization
Data is currently formatted on the ESP32 using ASCII float conversion (`dtostrf`) and transmitted across the raw socket stream:

```cpp
char payload[16];
dtostrf(Temp, 6, 2, payload); // Formats temperature float into character array
send(socket_fd, payload, strlen(payload), 0);
```

## ⚙️ Build & Compilation Guide

* **Microsoft Visual C++ Compiler (MSVC / Developer Command Prompt)

* ***To compile the native Windows server or client using MSVC's commandline compiler (cl), link directly against the Windows Sockets library (ws2_32.lib):

### Compile Server:
```cmd
cl server.cpp /EHsc /Fe:server.exe ws2_32.lib
```
### Compile Client:
```cmd
cl client.cpp /EHsc /Fe:client.exe ws2_32.lib
```
### GCC / MinGW Compiler (g++)

* ***If compiling via GCC on Windows or Linux:

### Compile Server:
```cmd
g++ -o server.exe server.cpp -lws2_32
```

## 🏃 Running the Pipeline

* **Launch the Server: Run the compiled binary inside your terminal, specifying the listening port:
```cmd
server.exe 8088
```

### Execute Microcontroller Firmware:
* ***Open the project inside the Wokwi Simulation Environment.
* ***Ensure setup() resolves the gateway address using getaddrinfo("host.wokwi.internal", "8088", &hints, &res).
* ***Start the simulation to establish the socket connection and observe live binary metric parsing in your desktop server console!

## 🔮 Upcoming Enhancements

* **Custom Memory Pool Integration: Intercepting incoming socket buffers with a custom, high-performance C++ arena pool allocator to eliminate OS-level heap fragmentation during high-frequency packet intake.
* **Multi-Threaded Worker Pool: Transitioning client connection handling to a thread pool model using std::thread and std::mutex synchronization primitives.
