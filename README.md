# Ultra-Low Latency System

Un sistema de comunicación de ultra-baja latencia diseñado en C++ para Windows que logra tiempos de respuesta menores a 1 milisegundo.

## 🎯 Objetivo

Este proyecto demuestra la capacidad de diseñar, implementar y optimizar una arquitectura de software enfocada en reducir drásticamente la latencia de comunicación. El sistema responde a estímulos (mensajes) con respuestas casi inmediatas, logrando latencias preferiblemente menores a un milisegundo.

## 🏗️ Arquitectura

### Componentes Principales

1. **Servidor Ultra-Latency** (`UltraLatencyServer`)
   - Utiliza **I/O Completion Ports (IOCP)** de Windows para máxima performance
   - Arquitectura multi-thread optimizada para concurrencia
   - Configuración de sockets optimizada para baja latencia
   - Medición de latencia de alta precisión

2. **Cliente Ultra-Latency** (`UltraLatencyClient`)
   - Cliente optimizado para testing y medición de latencia
   - Soporte para conexiones concurrentes
   - Múltiples modos de testing (batch, continuo, concurrente)
   - Análisis estadístico detallado

3. **Sistema de Medición de Alta Precisión**
   - Timer de alta resolución usando `QueryPerformanceCounter`
   - Medición en nanosegundos
   - Estadísticas completas (min, max, avg, percentiles)

### Optimizaciones Implementadas

- **TCP_NODELAY**: Elimina el algoritmo de Nagle para reducir latencia
- **Buffer Sizes**: Buffers optimizados de 64KB para send/receive
- **IOCP**: I/O Completion Ports para I/O asíncrono eficiente
- **Multi-threading**: Threads optimizados para el número de cores disponibles
- **Memory Management**: Gestión eficiente de memoria sin copias innecesarias
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

### 1. Ejecutar el Servidor

```bash
# Servidor básico
./server.exe

# Servidor con configuración personalizada
./server.exe --host 0.0.0.0 --port 8080
```

El servidor mostrará estadísticas en tiempo real cada 5 segundos.

### 2. Ejecutar Tests con el Cliente

```bash
# Test básico (1000 requests)
./client.exe

# Test con 10,000 requests y 4 conexiones concurrentes
./client.exe --requests 10000 --concurrent 4

# Test continuo por 30 segundos a 1000 req/sec
./client.exe --duration 30 --rate 1000

# Test personalizado
./client.exe --request "ping" --response "pong" --requests 5000

# Guardar resultados en CSV
./client.exe --requests 10000 --output results.csv
```

### 3. Test Integrado Completo

```bash
# Ejecuta todos los tests automáticamente
./latency_test.exe
```

Este test ejecuta:
- Test de request único
- Test ping-pong
- Test batch (1000 requests)
- Test concurrente (4 threads, 1000 requests)
- Test continuo (10 segundos, 1000 req/sec)

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

## 📈 Análisis de Performance

### Métricas Clave

1. **Latencia Mínima**: Tiempo de respuesta más rápido
2. **Latencia Promedio**: Latencia media de todas las requests
3. **Percentiles**: P50, P95, P99, P99.9 para análisis de distribución
4. **Throughput**: Requests por segundo procesadas
5. **Success Rate**: Porcentaje de requests exitosas

### Interpretación de Resultados

- **< 100 μs**: Excelente performance
- **100-500 μs**: Buena performance
- **500-1000 μs**: Performance aceptable
- **> 1000 μs**: Necesita optimización

## 🐛 Troubleshooting

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

- [Windows IOCP Documentation](https://docs.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports)
- [High-Performance Networking](https://docs.microsoft.com/en-us/windows/win32/winsock/high-performance-winsock-applications)
- [QueryPerformanceCounter](https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter)

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crear una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abrir un Pull Request

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Ver `LICENSE` para más detalles.

## 🏆 Logros

Este sistema ha sido diseñado para demostrar:

- ✅ Arquitectura de software optimizada para ultra-baja latencia
- ✅ Implementación eficiente usando tecnologías nativas de Windows
- ✅ Medición precisa de latencia en nanosegundos
- ✅ Escalabilidad con múltiples conexiones concurrentes
- ✅ Análisis estadístico completo de performance

**¡El objetivo es lograr latencias consistentemente menores a 1 milisegundo!** 