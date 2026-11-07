# Sistema de Gestión Logística Post-Desastre (UNGRD)
**Universidad EAFIT — Estructuras de Datos y Algoritmos (EDA)**
**Proyecto Final / Fase 3: Optimización Avanzada y Enrutamiento Vial**

---

## 1. Resumen Ejecutivo y Contexto del Reto Humanitario

Este sistema de software ha sido desarrollado en **C++** para la Unidad Nacional para la Gestión del Riesgo de Desastres (UNGRD). Su objetivo es automatizar y optimizar la cadena de suministro en escenarios de emergencia post-desastre (como sismos o inundaciones), gestionando el ciclo de vida de donaciones masivas, priorizando la atención de municipios afectados mediante triaje automatizado, y calculando rutas de distribución terrestres eficientes bajo restricciones geográficas y temporales.

El proyecto implementa estructuras de datos avanzadas desde cero, garantizando un rendimiento óptimo, manejo seguro de memoria y resiliencia ante fallos en tiempo de ejecución.

---

## 2. Arquitectura General y Módulos del Sistema

El proyecto se encuentra modularizado en los siguientes componentes clave dentro del directorio `Logica/` y `Modelos/`:

*   **`Modelos.h`**: Define las entidades estructurales base (`Donante`, `LoteAyuda`, `CentroAcopio`, `MunicipioAfectado`).
*   **`DataLoader.h / .cpp`**: Módulo de I/O encargado de la lectura, parsing y validación defensiva de archivos CSV (detección de valores nulos, formatos de fecha y rangos numéricos inválidos).
*   **`ColaPrioridad.h / .cpp`**: Montículo de Máximos (*Max-Heap*) implementado desde cero para el triaje municipal dinámico.
*   **`ArbolAVL.h / .cpp`**: Árbol Binario de Búsqueda Balanceado (AVL) indexado por fecha de caducidad para consultas por rango rápidas.
*   **`DeliveryGraph.h / .cpp`**: Grafo ponderado con listas de adyacencia y el algoritmo de Dijkstra para el cálculo de rutas óptimas y enrutamiento multipunto en convoy.
*   **`SistemaLogistico.h / .cpp`**: Fachada principal que unifica Hash Maps, Heaps, Árboles AVL y Grafos para gestionar la lógica de negocio y las alertas tempranas.
*   **`main.cpp`**: Interfaz de consola ejecutiva con menús interactivos, control de errores de entrada en `std::cin` y códigos de escape ANSI.

---

## 3. Justificación Arquitectónica y Complejidad Asintótica (Big-O)

El rendimiento del sistema se fundamenta en el análisis asintótico de sus estructuras de datos:

| Estructura de Datos | Uso en el Sistema | Complejidad Temporal (Worst/Average) | Complejidad Espacial |
| :--- | :--- | :--- | :--- |
| **Tablas Hash (`std::unordered_map`)** | Inventario de suministros, donantes y centros de acopio. | **$O(1)$** amortizado (búsqueda e inserción) | **$O(n)$** |
| **Montículo de Máximos (`ColaPrioridad`)** | Triaje de municipios por nivel de urgencia y necesidades. | **$O(\log n)$** (inserción y extracción) | **$O(n)$** |
| **Árbol AVL (`ArbolAVL`)** | Indexación y consultas por rango de fechas de caducidad. | **$O(\log n + k)$** (búsqueda y rangos) | **$O(n)$** |
| **Grafo Ponderado (`DeliveryGraph`)** | Red vial nacional y cálculo de rutas con Dijkstra / Heurística. | **$O(E \log V)$** (Dijkstra) | **$O(V + E)$** |

---

## 4. Plan de Pruebas Unitarias y Benchmarking

El sistema incluye una **Suite de Benchmarking Automatizado (Opción 16 del menú)** que evalúa casos límite y contrasta el rendimiento frente a alternativas ingenuas con $N = 10,000$ elementos:

1.  **Prueba de Consistencia Post-Despacho:** Valida que un lote donado e inmediatamente despachado sea eliminado correctamente del Hash Map y del Árbol AVL, evitando asignaciones fantasma.
2.  **Benchmarking Asintótico (`<chrono>`):** Mide en nanosegundos el tiempo de búsqueda en listas lineales $O(n)$ frente a Hash Maps $O(1)$, y el triaje por barrido lineal frente a nuestro Max-Heap $O(\log n)$, demostrando la superioridad de las estructuras optimizadas.

---

## 5. Compilación y Ejecución

Para compilar y ejecutar el proyecto desde la terminal (compatible con Linux / WSL), ubíquese en la carpeta `Logica/` y ejecute los siguientes comandos:

```bash
# Compilación integral del proyecto
g++ main.cpp DataLoader.cpp ColaPrioridad.cpp DeliveryGraph.cpp ArbolAVL.cpp SistemaLogistico.cpp -o proyecto_final

# Ejecución del sistema interactivo
./proyecto_final
```

---

## 6. Anexo de Transparencia Técnica de Inteligencia Artificial (EAFIT)

En el desarrollo de este proyecto se empleó la asistencia de Inteligencia Artificial (Gemini) con un enfoque de colaboración técnica y optimización de código de nivel senior:

*   **Nivel de Intervención y Toma de Decisiones:** La arquitectura general, el diseño estructural y las decisiones de negocio fueron concebidas y dirigidas en su totalidad por el equipo de desarrollo. La IA se utilizó como una herramienta de consulta, refinamiento y aseguramiento de la calidad del código.
*   **Dinámica de Trabajo (Estilo Coworking):** La interacción se basó en un flujo iterativo mediante prompts de validación técnica directa como:
    *   *«Esta es mi arquitectura ideal para el grafo multipunto, ¿le mejorarías algo?»*
    *   *«¿Cómo optimizarías la inserción en el Árbol AVL para normalizar las cadenas de fecha a claves enteras sin afectar el rendimiento?»*
*   **Validación y Dominio Técnico:** A partir de las sugerencias aportadas por el modelo, el equipo evaluó, adaptó e integró cada componente. Declaramos formalmente que **comprendemos y dominamos la totalidad del código fuente línea por línea**, ya que la estructura inicial y la lógica principal partieron de nuestra autoría, asumiendo total responsabilidad para la sustentación ante el comité evaluador de EAFIT.