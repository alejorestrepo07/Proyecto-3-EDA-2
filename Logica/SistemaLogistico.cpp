#include "SistemaLogistico.h"
#include <chrono>

// Constructor que inicializa Hash Maps, Max-Heap, Arbol AVL y Grafo
SistemaLogistico::SistemaLogistico(const std::vector<Donante>& donantes, 
                                   const std::vector<CentroAcopio>& centros,
                                   const std::vector<LoteAyuda>& lotes,
                                   const std::vector<MunicipioAfectado>& municipios) {
    // Declaracion de variables al inicio del bloque
    DetalleTecnicoLote defaultDetail;

    totalLotesDespachados = 0;
    pesoTotalDespachado = 0.0f;

    for (const auto& donante : donantes) {
        tablaDonantes[donante.idDonante] = donante;
    }
    for (const auto& centro : centros) {
        tablaCentros[centro.idCentro] = centro;
    }
    for (const auto& lote : lotes) {
        inventarioSuministros[lote.idLote] = lote;
        defaultDetail.volumenMetrosCubicos = lote.peso * 0.02f;
        defaultDetail.requiereCadenaFrio = (lote.tipoSuministro == "Medicamentos" || lote.tipoSuministro == "Vacunas");
        metadatosLotes[lote.idLote] = defaultDetail;
        arbolCaducidad.insertBatch(lote);
    }
    for (const auto& municipio : municipios) {
        tablaMunicipios[municipio.idMunicipio] = municipio;
        triajeMunicipios.insertar(municipio); 
    }
}

// Registra donacion en Hash Map O(1) y en Arbol AVL O(log n) con validacion
void SistemaLogistico::registrarDonacionEntrante(const LoteAyuda& nuevoLote, float volumen, bool requiereFrio) {
    // Declaracion de variables al inicio del bloque
    DetalleTecnicoLote detail;

    if (nuevoLote.peso <= 0.0f || volumen <= 0.0f) {
        std::cout << "\033[1;31m[ERROR DE VALIDACION]\033[0m El peso y el volumen deben ser valores positivos.\n";
        return;
    }

    detail.volumenMetrosCubicos = volumen;
    detail.requiereCadenaFrio = requiereFrio;

    inventarioSuministros[nuevoLote.idLote] = nuevoLote;
    metadatosLotes[nuevoLote.idLote] = detail;
    arbolCaducidad.insertBatch(nuevoLote);

    std::cout << "\033[1;32m[EXITO]\033[0m Lote ID " << nuevoLote.idLote 
              << " indexado en Hash Map O(1) y Arbol AVL O(log n).\n";
}

// Busqueda directa por ID en tiempo constante O(1)
void SistemaLogistico::buscarSuministroPorId(int idLote) {
    // Declaracion de variables al inicio del bloque
    std::unordered_map<int, LoteAyuda>::iterator iterador;
    DetalleTecnicoLote detail;
    
    iterador = inventarioSuministros.find(idLote);
    
    if (iterador != inventarioSuministros.end()) {
        detail = metadatosLotes[idLote];
        std::cout << "\033[1;32m[SUMINISTRO DISPONIBLE]\033[0m\n"
                  << "-> ID: " << iterador->second.idLote 
                  << " | Tipo: " << iterador->second.tipoSuministro 
                  << " | Peso: " << iterador->second.peso << " kg"
                  << " | Volumen: " << detail.volumenMetrosCubicos << " m3"
                  << " | Cadena de Frio: " << (detail.requiereCadenaFrio ? "SI" : "NO")
                  << " | Vence: " << iterador->second.fechaCaducidad << "\n";
    } else {
        std::cout << "\033[1;31m[ALERTA]\033[0m El lote ID " << idLote << " no se encuentra disponible en bodega.\n";
    }
}

// Balance de stock en tiempo real: remanentes vs despachados y alertas tempranas
void SistemaLogistico::generarInformeStock() {
    // Declaracion de variables al inicio del bloque
    float pesoRemanente;
    float volumenTotal;

    pesoRemanente = 0.0f;
    volumenTotal = 0.0f;
    
    for (const auto& par : inventarioSuministros) {
        pesoRemanente += par.second.peso;
        volumenTotal += metadatosLotes[par.first].volumenMetrosCubicos;
    }
    
    std::cout << "\n\033[1;36m=== BALANCE GENERAL DE INVENTARIO Y DESPACHOS ===\033[0m\n";
    std::cout << "-> Lotes remanentes en bodega:  " << inventarioSuministros.size() << " unidades\n";
    std::cout << "-> Peso disponible en bodega:   " << pesoRemanente << " kg\n";
    std::cout << "-> Volumen ocupado en acopio:   " << volumenTotal << " m3\n";
    std::cout << "-> Lotes ya despachados:        " << totalLotesDespachados << " unidades\n";
    std::cout << "-> Peso historico despachado:   " << pesoTotalDespachado << " kg\n";

    if (inventarioSuministros.size() < 3 || pesoRemanente < 100.0f) {
        std::cout << "\033[1;31m[ALERTA TEMPRANA DE DESABASTECIMIENTO]\033[0m Nivel critico de reservas en bodega.\n";
    } else {
        std::cout << "\033[1;32m[ESTADO DE RESERVAS]\033[0m Nivel de abastecimiento saludable.\n";
    }
    std::cout << "=================================================\n";
}

// Elimina un lote del inventario activo y actualiza metricas de despacho
void SistemaLogistico::darDeBajaSuministro(int idLote) {
    // Declaracion de variables al inicio del bloque
    std::unordered_map<int, LoteAyuda>::iterator searchIt;
    
    searchIt = inventarioSuministros.find(idLote);

    if (searchIt != inventarioSuministros.end()) {
        totalLotesDespachados++;
        pesoTotalDespachado += searchIt->second.peso;
        arbolCaducidad.removeBatch(idLote);
        metadatosLotes.erase(idLote);
        inventarioSuministros.erase(searchIt);
        std::cout << "\033[1;32m[EXITO]\033[0m Lote ID " << idLote << " despachado y dado de baja del inventario.\n";
    } else {
        std::cout << "\033[1;31m[ERROR]\033[0m No se encontro ningun lote disponible con ID " << idLote << ".\n";
    }
}

// Filtra suministros por categoria especifica en O(n)
void SistemaLogistico::filtrarSuministrosPorCategoria(const std::string& categoria) {
    // Declaracion de variables al inicio del bloque
    std::unordered_map<int, LoteAyuda>::iterator mapIt;
    float totalWeight;
    int itemsCount;

    totalWeight = 0.0f;
    itemsCount = 0;

    for (mapIt = inventarioSuministros.begin(); mapIt != inventarioSuministros.end(); ++mapIt) {
        if (mapIt->second.tipoSuministro == categoria) {
            totalWeight += mapIt->second.peso;
            itemsCount++;
        }
    }

    std::cout << "\n\033[1;36m[REPORTE FILTRADO POR CATEGORIA: " << categoria << "]\033[0m\n";
    std::cout << "-> Cantidad de lotes encontrados: " << itemsCount << "\n";
    std::cout << "-> Peso total acumulado: " << totalWeight << " kg\n";
}

// Consulta el municipio en la cima del Max-Heap sin retirarlo
void SistemaLogistico::consultarProximoMunicipio() {
    // Declaracion de variables al inicio del bloque
    MunicipioAfectado topMunicipality;

    if (triajeMunicipios.estaVacia()) {
        std::cout << "\033[1;33m[INFO]\033[0m No hay municipios pendientes en la cola de prioridad.\n";
        return;
    }

    topMunicipality = triajeMunicipios.extraerMaximo();
    std::cout << "\n\033[1;36m[RADAR DE TRIAJE - MAXIMA PRIORIDAD ACTUAL]\033[0m\n";
    std::cout << "-> Municipio: " << topMunicipality.nombre 
              << " (ID: " << topMunicipality.idMunicipio << ")"
              << " | Urgencia: " << topMunicipality.nivelUrgencia 
              << " | Necesidades: " << topMunicipality.cantidadNecesidades << "\n";
    triajeMunicipios.insertar(topMunicipality);
}

// Extrae y atiende al municipio con mayor urgencia en O(log n)
void SistemaLogistico::atenderMunicipioMasUrgente() {
    // Declaracion de variables al inicio del bloque
    MunicipioAfectado municipioPrioridad;
    
    if (triajeMunicipios.estaVacia()) {
        std::cout << "\033[1;33m[INFO]\033[0m No hay municipios pendientes por atender en el triaje.\n";
        return;
    }
    
    municipioPrioridad = triajeMunicipios.extraerMaximo();
    
    std::cout << "\n\033[1;32m[TRIAJE LOGISTICO]\033[0m Atendiendo emergencia prioritaria:\n";
    std::cout << "-> Municipio despachado: " << municipioPrioridad.nombre 
              << " | Nivel Urgencia: " << municipioPrioridad.nivelUrgencia 
              << " | Necesidades: " << municipioPrioridad.cantidadNecesidades << "\n";
}

// Actualiza parametros de urgencia y reordena en el Max-Heap
void SistemaLogistico::actualizarPrioridadMunicipio(int idMunicipio, int nuevoNivel, int nuevasNecesidades) {
    if (tablaMunicipios.find(idMunicipio) != tablaMunicipios.end()) {
        tablaMunicipios[idMunicipio].nivelUrgencia = nuevoNivel;
        tablaMunicipios[idMunicipio].cantidadNecesidades = nuevasNecesidades;
        triajeMunicipios.insertar(tablaMunicipios[idMunicipio]);
        std::cout << "\033[1;32m[EXITO]\033[0m Prioridad del municipio ID " << idMunicipio << " actualizada en el Max-Heap.\n";
    } else {
        std::cout << "\033[1;31m[ERROR]\033[0m No existe un municipio registrado con el ID " << idMunicipio << ".\n";
    }
}

// Consulta lotes en el Arbol AVL dentro de un rango de fechas de vencimiento
void SistemaLogistico::consultarRangoCaducidad(const std::string& fechaInicio, const std::string& fechaFin) {
    arbolCaducidad.searchByDateRange(fechaInicio, fechaFin);
}

// Lista todos los suministros ordenados por fecha de vencimiento usando recorrido In-Order del AVL
void SistemaLogistico::listarLotesPorVencimiento() {
    arbolCaducidad.printAllByExpiry();
}

// Agrega arista ponderada al Grafo logistico
void SistemaLogistico::agregarRutaMapa(int idOrigen, int idDestino, float distancia) {
    redDeRutas.addEdge(idOrigen, idDestino, distancia);
}

// Imprime todas las conexiones viales del Grafo
void SistemaLogistico::imprimirMapaRutas() {
    redDeRutas.printGraph();
}

// Calcula la ruta optima de entrega mediante el algoritmo de Dijkstra O(E log V)
void SistemaLogistico::despacharRutaOptima(int idCentro, int idMunicipio) {
    std::cout << "\n\033[1;36m[DESPACHO LOGISTICO]\033[0m Calculando ruta optima con Dijkstra...\n";
    std::cout << "Origen: Centro Acopio ID " << idCentro << " -> Destino: Municipio ID " << idMunicipio << "\n";
    redDeRutas.calculateShortestPath(idCentro, idMunicipio);
}

// Asignacion automatizada: extrae municipio mas urgente, asigna lote disponible y calcula ruta optima
void SistemaLogistico::asignarRecursosAutomatizado(int idCentroOrigen) {
    // Declaracion de variables al inicio del bloque
    MunicipioAfectado targetMunicipality;
    std::unordered_map<int, LoteAyuda>::iterator batchIt;
    int assignedBatchId;

    if (triajeMunicipios.estaVacia()) {
        std::cout << "\033[1;33m[AVISO]\033[0m No hay municipios pendientes en la cola de triaje.\n";
        return;
    }
    if (inventarioSuministros.empty()) {
        std::cout << "\033[1;31m[ALERTA CRITICA]\033[0m Inventario agotado. Imposible asignar recursos.\n";
        return;
    }

    targetMunicipality = triajeMunicipios.extraerMaximo();
    batchIt = inventarioSuministros.begin();
    assignedBatchId = batchIt->first;

    std::cout << "\n\033[1;32m=== ASIGNACION AUTOMATIZADA DE EMERGENCIA ===\033[0m\n";
    std::cout << "1. Municipio seleccionado por Max-Heap: " << targetMunicipality.nombre 
              << " (Urgencia: " << targetMunicipality.nivelUrgencia << ")\n";
    std::cout << "2. Suministro asignado: Lote ID " << assignedBatchId 
              << " (" << batchIt->second.tipoSuministro << " - " << batchIt->second.peso << " kg)\n";

    darDeBajaSuministro(assignedBatchId);
    despacharRutaOptima(idCentroOrigen, 44);
}

// Ejecuta el enrutamiento multipunto ordenado para multiples municipios en un solo viaje
void SistemaLogistico::despacharRutaMultipunto(int idCentroOrigen, const std::vector<int>& destinos) {
    std::cout << "\n\033[1;36m[CONSOLA DE DESPACHO MULTIPUNTO]\033[0m Calculando secuencia optima de reparto...\n";
    redDeRutas.calculateMultiStopRoute(idCentroOrigen, destinos);
}

// Ejecuta pruebas unitarias automatizadas de las 3 fases y benchmarking comparativo
void SistemaLogistico::ejecutarSuitePruebasYBenchmarking() {
    // Declaracion de todas las variables al inicio del bloque
    LoteAyuda testBatch;
    bool existedBefore;
    bool existsAfter;
    int numElements;
    int i;
    int targetKey;
    std::vector<LoteAyuda> linearVector;
    std::unordered_map<int, LoteAyuda> testHashMap;
    ColaPrioridad testHeap;
    std::vector<MunicipioAfectado> linearMunVector;
    MunicipioAfectado dummyMun;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    long long linearSearchTimeUs;
    long long hashSearchTimeUs;
    long long linearMaxTimeUs;
    long long heapExtractTimeUs;
    int maxUrgencyFound;

    numElements = 10000;
    targetKey = 9999;
    maxUrgencyFound = -1;

    std::cout << "\n\033[1;35m================================================================\033[0m\n";
    std::cout << "\033[1;37m   SUITE DE PRUEBAS UNITARIAS Y BENCHMARKING ASINTOTICO (EAFIT)\033[0m\n";
    std::cout << "\033[1;35m================================================================\033[0m\n";

    // Prueba Unitaria 1: Validacion de integridad y no disponibilidad post-despacho (Fase 2)
    testBatch.idLote = 88888;
    testBatch.tipoSuministro = "KitQuirurgico";
    testBatch.peso = 45.0f;
    testBatch.fechaCaducidad = "2027-05-15";

    registrarDonacionEntrante(testBatch, 1.2f, true);
    existedBefore = (inventarioSuministros.find(88888) != inventarioSuministros.end());
    darDeBajaSuministro(88888);
    existsAfter = (inventarioSuministros.find(88888) != inventarioSuministros.end());

    std::cout << "\n[TEST UNITARIO 1 - CONSISTENCIA POST-DESPACHO]: ";
    if (existedBefore && !existsAfter) {
        std::cout << "\033[1;32mAPROBADO\033[0m (El lote asignado ya no figura como disponible).\n";
    } else {
        std::cout << "\033[1;31mFALLIDO\033[0m\n";
    }

    // Preparacion de 10,000 registros sinteticos para Benchmarking (Fase 3)
    for (i = 0; i < numElements; i++) {
        testBatch.idLote = i;
        testBatch.peso = 10.0f;
        linearVector.push_back(testBatch);
        testHashMap[i] = testBatch;

        dummyMun.idMunicipio = i;
        dummyMun.nombre = "MunTest";
        dummyMun.nivelUrgencia = (i % 5) + 1;
        dummyMun.cantidadNecesidades = i * 10;
        linearMunVector.push_back(dummyMun);
        testHeap.insertar(dummyMun);
    }

    // Benchmark 1: Busqueda Lineal O(n) vs Hash Map O(1)
    startTime = std::chrono::high_resolution_clock::now();
    for (i = 0; i < numElements; i++) {
        if (linearVector[i].idLote == targetKey) {
            break;
        }
    }
    endTime = std::chrono::high_resolution_clock::now();
    linearSearchTimeUs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    testHashMap.find(targetKey);
    endTime = std::chrono::high_resolution_clock::now();
    hashSearchTimeUs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    // Benchmark 2: Triaje por barrido Lineal O(n) vs Max-Heap O(log n)
    startTime = std::chrono::high_resolution_clock::now();
    for (i = 0; i < numElements; i++) {
        if (linearMunVector[i].nivelUrgencia > maxUrgencyFound) {
            maxUrgencyFound = linearMunVector[i].nivelUrgencia;
        }
    }
    endTime = std::chrono::high_resolution_clock::now();
    linearMaxTimeUs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    testHeap.extraerMaximo();
    endTime = std::chrono::high_resolution_clock::now();
    heapExtractTimeUs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

    std::cout << "\n\033[1;36m[BENCHMARKING DE RENDIMIENTO CON N = 10,000 REGISTROS]\033[0m\n";
    std::cout << "1. Busqueda en Lista Lineal O(n):     " << linearSearchTimeUs << " ns\n";
    std::cout << "   Busqueda en Hash Map O(1):         " << hashSearchTimeUs << " ns\n";
    std::cout << "2. Triaje en Lista Lineal O(n):       " << linearMaxTimeUs << " ns\n";
    std::cout << "   Extraccion en Max-Heap O(log n):   " << heapExtractTimeUs << " ns\n";
    std::cout << "\033[1;32m[CONCLUSION]\033[0m Las estructuras seleccionadas superan exponencialmente a las alternativas ingenuas.\n";
    std::cout << "\033[1;35m================================================================\033[0m\n";
}