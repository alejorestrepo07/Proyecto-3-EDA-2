#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "DataLoader.h"
#include "SistemaLogistico.h"

// Archivo principal interactivo - Sistema Integral de Donaciones EAFIT
int main() {
    // Declaracion de todas las variables al inicio del bloque
    std::vector<Donante> donorsList;
    std::vector<LoteAyuda> batchesList;
    std::vector<CentroAcopio> centersList;
    std::vector<MunicipioAfectado> municipalitiesList;
    std::vector<int> multiTargetIds;
    
    int userOption;
    int searchId;
    int originId;
    int destinationId;
    int numStops;
    int stopId;
    int idx;
    float routeDistance;
    int deleteId;
    std::string searchCategory;
    std::string startDate;
    std::string endDate;
    
    int newBatchId;
    std::string newType;
    float newWeight;
    float newVolume;
    int coldChainInput;
    std::string newExpiry;
    LoteAyuda newBatch;
    
    int targetMunicipalityId;
    int updatedUrgency;
    int updatedNeeds;

    std::cout << "\033[1;36mInicializando el Sistema de Gestion Logistica de Emergencias...\033[0m\n\n";

    // Carga de bases de datos desde CSV (Fase 1)
    donorsList = DataLoader::cargarDonantes("../Datos_CSV/donantes.csv");
    batchesList = DataLoader::cargarLotes("../Datos_CSV/lotes.csv");
    centersList = DataLoader::cargarCentros("../Datos_CSV/centros.csv");
    municipalitiesList = DataLoader::cargarMunicipios("../Datos_CSV/municipios.csv");

    SistemaLogistico sistema(donorsList, centersList, batchesList, municipalitiesList);

    // Configuracion de la Red Vial Colombiana:
    // Centro de Acopio Principal = ID 1 (Medellin)
    // Municipios del caso de prueba: 44 (mas cercano: 45 km), 27 (105 km), 60 (195 km), 10 (mas lejano: 315 km)
    sistema.agregarRutaMapa(1, 44, 45.0f);   // Acopio 1 -> Municipio 44 (45 km)
    sistema.agregarRutaMapa(44, 27, 60.0f);  // Municipio 44 -> Municipio 27 (Acumulado: 105 km)
    sistema.agregarRutaMapa(27, 60, 90.0f);  // Municipio 27 -> Municipio 60 (Acumulado: 195 km)
    sistema.agregarRutaMapa(60, 10, 120.0f); // Municipio 60 -> Municipio 10 (Acumulado: 315 km)
    
    // Rutas alternas directas mas largas (para que Dijkstra demuestre optimizacion)
    sistema.agregarRutaMapa(1, 27, 140.0f);
    sistema.agregarRutaMapa(1, 60, 260.0f);
    sistema.agregarRutaMapa(1, 10, 420.0f);

    // Conexiones adicionales con nodos 11, 20 y 21
    sistema.agregarRutaMapa(10, 20, 230.5f); 
    sistema.agregarRutaMapa(10, 21, 400.0f); 
    sistema.agregarRutaMapa(11, 21, 150.0f); 
    sistema.agregarRutaMapa(11, 20, 500.0f); 

    do {
        std::cout << "\n\033[1;34m==============================================================\033[0m\n";
        std::cout << "\033[1;37m       SISTEMA LOGISTICO POST-DESASTRE (UNGRD / EAFIT)\033[0m\n";
        std::cout << "\033[1;34m==============================================================\033[0m\n";
        std::cout << "  --- FASE 1 Y 2: INVENTARIO Y HASH MAPS O(1) ---\n";
        std::cout << "1. Balance general de stock, despachos y alertas tempranas\n";
        std::cout << "2. Buscar lote de suministro por ID unico (O(1))\n";
        std::cout << "3. Filtrar suministros por categoria especifica\n";
        std::cout << "4. Registrar nueva donacion entrante (Con volumen y frio)\n";
        std::cout << "5. Dar de baja (despachar) un suministro manualmente\n";
        std::cout << "\n  --- FASE 3: ARBOL BALANCEADO AVL (CADUCIDAD O(log n)) ---\n";
        std::cout << "6. Consultar lotes por rango de fecha de expiracion (AVL)\n";
        std::cout << "7. Listar todos los suministros por orden de vencimiento\n";
        std::cout << "\n  --- FASE 3: TRIAJE CON MAX-HEAP O(log n) ---\n";
        std::cout << "8. Radar: Consultar el proximo municipio mas urgente\n";
        std::cout << "9. Atender municipio mas urgente (Extraer del Max-Heap)\n";
        std::cout << "10. Actualizar nivel de urgencia de un municipio\n";
        std::cout << "\n  --- FASE 3: GRAFOS, DIJKSTRA Y DESPACHO O(E log V) ---\n";
        std::cout << "11. Imprimir mapa de red vial colombiana\n";
        std::cout << "12. Agregar nueva carretera o ruta al Grafo\n";
        std::cout << "13. Calcular ruta optima a un destino (Dijkstra Simple)\n";
        std::cout << "14. Asignacion automatizada integral (Triaje + Stock + Ruta)\n";
        std::cout << "15. Despacho Multipunto Ordenado (Varios municipios en ruta)\n";
        std::cout << "\n  --- CALIDAD Y RENDIMIENTO (RUBRICA DE PRUEBAS) ---\n";
        std::cout << "16. Ejecutar Suite de Pruebas Unitarias y Benchmarking\n";
        std::cout << "\033[1;34m==============================================================\033[0m\n";
        std::cout << "0. Finalizar sesion y apagar el sistema\n";
        std::cout << "\033[1;34m==============================================================\033[0m\n";
        std::cout << "Seleccione una operacion administrativa: ";
        std::cin >> userOption;

        // Blindaje contra entradas no numericas
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\033[1;31m[ERROR] Entrada invalida. Ingrese unicamente valores numericos.\033[0m\n";
            userOption = -1;
            continue;
        }

        switch (userOption) {
            case 1:
                sistema.generarInformeStock();
                break;
            case 2:
                std::cout << "\nIngrese el ID del lote: ";
                std::cin >> searchId;
                sistema.buscarSuministroPorId(searchId);
                break;
            case 3:
                std::cout << "\nIngrese la categoria a buscar (ej. Agua, Alimentos, Medicamentos): ";
                std::cin >> searchCategory;
                sistema.filtrarSuministrosPorCategoria(searchCategory);
                break;
            case 4:
                std::cout << "\n\033[1;32m[REGISTRO DE LOTE]\033[0m ID Lote: ";
                std::cin >> newBatchId;
                std::cout << "Tipo de suministro: ";
                std::cin >> newType;
                std::cout << "Peso (kg): ";
                std::cin >> newWeight;
                std::cout << "Volumen (m3): ";
                std::cin >> newVolume;
                std::cout << "Requiere cadena de frio (1 = Si, 0 = No): ";
                std::cin >> coldChainInput;
                std::cout << "Fecha de caducidad (AAAA-MM-DD o DD/MM/AAAA): ";
                std::cin >> newExpiry;
                
                newBatch.idLote = newBatchId;
                newBatch.tipoSuministro = newType;
                newBatch.peso = newWeight;
                newBatch.fechaCaducidad = newExpiry;
                
                sistema.registrarDonacionEntrante(newBatch, newVolume, (coldChainInput == 1));
                break;
            case 5:
                std::cout << "\nIngrese el ID del lote a despachar: ";
                std::cin >> deleteId;
                sistema.darDeBajaSuministro(deleteId);
                break;
            case 6:
                std::cout << "\nIngrese fecha inicial (ej. 2026-01-01): ";
                std::cin >> startDate;
                std::cout << "Ingrese fecha final (ej. 2027-12-31): ";
                std::cin >> endDate;
                sistema.consultarRangoCaducidad(startDate, endDate);
                break;
            case 7:
                sistema.listarLotesPorVencimiento();
                break;
            case 8:
                sistema.consultarProximoMunicipio();
                break;
            case 9:
                sistema.atenderMunicipioMasUrgente();
                break;
            case 10:
                std::cout << "\n\033[1;32m[ACTUALIZACION]\033[0m ID Municipio: ";
                std::cin >> targetMunicipalityId;
                std::cout << "Nuevo nivel urgencia (1-5): ";
                std::cin >> updatedUrgency;
                std::cout << "Nuevas necesidades: ";
                std::cin >> updatedNeeds;
                sistema.actualizarPrioridadMunicipio(targetMunicipalityId, updatedUrgency, updatedNeeds);
                break;
            case 11:
                sistema.imprimirMapaRutas();
                break;
            case 12:
                std::cout << "\n\033[1;32m[NUEVA RUTA]\033[0m ID Ubicacion Origen: ";
                std::cin >> originId;
                std::cout << "ID Ubicacion Destino: ";
                std::cin >> destinationId;
                std::cout << "Distancia (km): ";
                std::cin >> routeDistance;
                sistema.agregarRutaMapa(originId, destinationId, routeDistance);
                std::cout << "\033[1;32m[EXITO]\033[0m Conexion vial registrada en el Grafo.\n";
                break;
            case 13:
                std::cout << "\n\033[1;36m[DIJKSTRA]\033[0m ID Centro de salida (ej. 1 o 10): ";
                std::cin >> originId;
                std::cout << "ID Municipio entrega (ej. 44, 27, 60, 10, 20): ";
                std::cin >> destinationId;
                sistema.despacharRutaOptima(originId, destinationId);
                break;
            case 14:
                std::cout << "\nIngrese ID del Centro de Acopio despachador (ej. 1): ";
                std::cin >> originId;
                sistema.asignarRecursosAutomatizado(originId);
                break;
            case 15:
                multiTargetIds.clear();
                std::cout << "\n\033[1;36m[DESPACHO MULTIPUNTO ORDENADO]\033[0m\n";
                std::cout << "Ingrese ID del Centro de Acopio de salida (ej. 1): ";
                std::cin >> originId;
                std::cout << "Cuantos municipios desea visitar en este recorrido? (ej. 4): ";
                std::cin >> numStops;
                for (idx = 0; idx < numStops; idx++) {
                    std::cout << "  -> Ingrese ID del Municipio #" << (idx + 1) << " (ej. 10, 60, 44, 27): ";
                    std::cin >> stopId;
                    multiTargetIds.push_back(stopId);
                }
                sistema.despacharRutaMultipunto(originId, multiTargetIds);
                break;
            case 16:
                sistema.ejecutarSuitePruebasYBenchmarking();
                break;
            case 0:
                std::cout << "\n\033[1;36mCerrando operaciones. Sesion finalizada con exito.\033[0m\n";
                break;
            default:
                std::cout << "\n\033[1;31m[ATENCION] Opcion no reconocida.\033[0m\n";
                break;
        }
    } while (userOption != 0);

    return 0;
}