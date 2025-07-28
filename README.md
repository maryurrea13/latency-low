# Low Latency System

Un sistema de comunicación de latencia baja en C++ para Windows que logra tiempos de respuesta menores a 1 milisegundo.

## 🎯 Objetivo

Diseñar, implementar y optimizar una arquitectura de software enfocada en reducir la latencia de comunicación. El sistema responde a estímulos (mensajes) con respuestas casi inmediatas, logrando latencias preferiblemente menores a un milisegundo.

## 🏗️ Arquitectura

### Componentes Principales

1. **Servidor Simple** (`SimpleServer`)
   - Implementación en `server.cpp` con arquitectura multi-thread
   - Configuración de sockets optimizada para baja latencia
   - Manejo de conexiones concurrentes
   - Respuesta automática a requests

2. **Cliente Simple** (`SimpleClient`)
   - Implementación en `client.cpp` para testing y medición
   - Medición de latencia de alta precisión
   - Múltiples modos de testing (batch, continuo)
   - Análisis estadístico de performance

3. **Sistema de Medición de Alta Precisión**
   - Timer de alta resolución usando `QueryPerformanceCounter`
   - Medición en nanosegundos
   - Estadísticas completas (min, max, avg, percentiles)

### Optimizaciones Implementadas

- **Multi-threading**: Threads optimizados para el número de cores disponibles
- **High-Resolution Timer**: Medición de latencia en nanosegundos
- **Socket Optimization**: Configuración optimizada de sockets
- **Memory Management**: Gestión eficiente de memoria
- **Compiler Optimizations**: Flags de optimización agresivos para máxima velocidad

## 📋 Requisitos

- **Sistema Operativo**: Windows 10/11 (x64)
- **Compilador**: Visual Studio 2019+ o MSVC compatible
- **CMake**: Versión 3.16 o superior
- **Hardware**: Procesador moderno con soporte para AVX2 (recomendado)

## 🚀 Compilación

### Prerrequisitos

1. Instalar Visual Studio 2019+ con soporte para C++
2. Instalar CMake 3.16+
3. Clonar el repositorio

### Pasos de Compilación

```bash
# Crear directorio de build
mkdir build
cd build

# Configurar con CMake
cmake .. -G "Visual Studio 16 2019" -A x64

# Compilar (Release mode para máxima performance)
cmake --build . --config Release
```

### Estructura de Archivos Generados

```
build/bin/Release/
├── server.exe          # Servidor principal
├── client.exe          # Cliente de testing
└── latency_test.exe    # Test integrado completo
```

## 🎮 Uso

### 🚀 Ejecución Manual del Sistema

#### **Paso 1: Iniciar el Servidor**

Abre una **primera ventana de PowerShell/CMD** y ejecuta:

```bash
# Navegar al directorio del proyecto
cd "C:\Users\Mary Urrea\Desktop\ultra-low-latency-system"

# Ejecutar el servidor
bin\server.exe
```

El servidor debería mostrar:
```
Simple server started on port 8080
Server running. Press Enter to stop...
```

#### **Paso 2: Ejecutar el Cliente (en otra ventana)**

Abre una **segunda ventana de PowerShell/CMD** y ejecuta:

```bash
# Navegar al directorio del proyecto
cd "C:\Users\Mary Urrea\Desktop\ultra-low-latency-system"

# Test básico (5 requests)
bin\client.exe --requests 5

# O test con más requests
bin\client.exe --requests 100

# O test personalizado
bin\client.exe --request "hello" --expected "respuesta" --requests 3
```

### 🎯 Comandos Específicos del Cliente

```bash
# Test básico
bin\client.exe --requests 5

# Test con mensaje personalizado
bin\client.exe --request "ping" --expected "pong" --requests 10

# Test de latencia completo
bin\client.exe --requests 1000

# Test continuo por 10 segundos
bin\client.exe --duration 10 --rate 1000

# Test con múltiples conexiones
bin\client.exe --requests 100 --concurrent 4
```

### 🧪 Usar el Test Automático

Si prefieres usar el sistema automático que ya funciona:

```bash
# Ejecutar test completo (inicia servidor + cliente automáticamente)
test.bat
```

### ⚠️ Posibles Problemas y Soluciones

#### **Si el servidor no inicia:**
- Verifica que el puerto 8080 esté libre
- Ejecuta como administrador si es necesario

#### **Si el cliente no se conecta:**
- Asegúrate de que el servidor esté ejecutándose
- Verifica que estén en la misma red (localhost)

#### **Si hay errores de DLL:**
- Los archivos DLL ya están en el directorio `bin/`
- Ejecuta desde el directorio raíz del proyecto

### 🎮 Ejemplo de Uso Completo

```bash
# Terminal 1 - Servidor
cd "C:\Users\Mary Urrea\Desktop\ultra-low-latency-system"
bin\server.exe

# Terminal 2 - Cliente
cd "C:\Users\Mary Urrea\Desktop\ultra-low-latency-system"
bin\client.exe --requests 10
```

### 📋 Comandos Avanzados

```bash
# Servidor con configuración personalizada
bin\server.exe --host 0.0.0.0 --port 8080

# Test con 10,000 requests y 4 conexiones concurrentes
bin\client.exe --requests 10000 --concurrent 4

# Test continuo por 30 segundos a 1000 req/sec
bin\client.exe --duration 30 --rate 1000

# Test personalizado
bin\client.exe --request "ping" --response "pong" --requests 5000

# Guardar resultados en CSV
bin\client.exe --requests 10000 --output results.csv
```

## 📊 Resultados Esperados

### Latencia Objetivo
- **Objetivo**: < 1ms (1000 μs)
- **Típico**: 50-500 μs
- **Óptimo**: < 100 μs

### Ejemplo de Salida

```
=== Test Results ===
Test Duration: 1250 ms
Total Requests: 1000
Successful: 1000
Failed: 0
Success Rate: 100.00%

Latency Statistics:
  Min: 45.123 μs
  Max: 234.567 μs
  Avg: 89.456 μs
  P50: 87.234 μs
  P95: 156.789 μs
  P99: 198.456 μs
  P99.9: 223.123 μs

Performance Metrics:
  Throughput: 800.0 req/sec

✓ ACHIEVED SUB-MILLISECOND LATENCY!
  Best latency: 45.123 μs
```

## 🔧 Configuración Avanzada

### Optimizaciones del Sistema

Para lograr la máxima performance, considera estas optimizaciones del sistema:

1. **Power Plan**: Configurar Windows en "High Performance"
2. **CPU Affinity**: Asignar procesos a cores específicos
3. **Network**: Usar conexión directa (no WiFi)
4. **Antivirus**: Excluir el directorio del proyecto
5. **Background Services**: Deshabilitar servicios innecesarios

### Parámetros de Configuración

#### Servidor
```cpp
ServerConfig config;
config.host = "127.0.0.1";
config.port = 8080;
config.thread_count = std::thread::hardware_concurrency();
config.tcp_nodelay = true;
config.send_buffer_size = 64 * 1024;  // 64KB
config.recv_buffer_size = 64 * 1024;  // 64KB
config.reuse_address = true;
```

#### Cliente
```cpp
ClientConfig config;
config.host = "127.0.0.1";
config.port = 8080;
config.tcp_nodelay = true;
config.send_buffer_size = 64 * 1024;
config.recv_buffer_size = 64 * 1024;
config.timeout_ms = 1000;
```

### Problemas Comunes

1. **Error de Conexión**
   - Verificar que el servidor esté ejecutándose
   - Comprobar firewall de Windows
   - Verificar puerto disponible

2. **Latencia Alta**
   - Ejecutar en modo Release
   - Verificar optimizaciones del sistema
   - Usar conexión directa (no WiFi)

3. **Errores de Compilación**
   - Verificar versión de Visual Studio
   - Actualizar CMake
   - Verificar dependencias

### Logs y Debugging

El sistema incluye logging detallado. Para debugging:

```bash
# Ejecutar en modo Debug
cmake --build . --config Debug
```

## 🔬 Casos de Uso

### Aplicaciones Financieras
- Trading de alta frecuencia
- Sistemas de matching engines
- Procesamiento de órdenes

### Gaming
- Servidores de juegos en tiempo real
- Sistemas de matchmaking
- Comunicación cliente-servidor

### IoT y Edge Computing
- Comunicación entre dispositivos
- Procesamiento de sensores
- Sistemas de control en tiempo real

## 📚 Referencias Técnicas

- [Windows Sockets Programming](https://docs.microsoft.com/en-us/windows/win32/winsock/)
- [High-Performance Networking](https://docs.microsoft.com/en-us/windows/win32/winsock/high-performance-winsock-applications)
- [High-Resolution Timer](https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps)

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crear una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abrir un Pull Request


**¡El objetivo es lograr latencias consistentemente menores a 1 milisegundo!** 

Archivos .bat

compile.bat:
✅ Compilación optimizada para máxima performance
✅ Verificación de dependencias (MinGW)
✅ Manejo de errores con mensajes claros
✅ Estructura organizada con directorio bin/

test.bat:
✅ Testing automático del sistema completo
✅ Verificación de componentes críticos
✅ Debugging visual con ventana del servidor
✅ Tests múltiples para validación completa