#ifndef SISTEMA_LOGISTICO_H
#define SISTEMA_LOGISTICO_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "../Modelos/Modelos.h"
#include "ColaPrioridad.h"
#include "DeliveryGraph.h"
#include "ArbolAVL.h"

// Estructura auxiliar para almacenar atributos extendidos (volumen y cadena de frio)
struct DetalleTecnicoLote {
    float volumenMetrosCubicos;
    bool requiereCadenaFrio;
};

// Clase principal que gestiona todas las estructuras de datos y logica de negocio
class SistemaLogistico {
private:
    std::unordered_map<int, Donante> tablaDonantes;
    std::unordered_map<int, CentroAcopio> tablaCentros;
    std::unordered_map<int, LoteAyuda> inventarioSuministros;
    std::unordered_map<int, DetalleTecnicoLote> metadatosLotes;
    std::unordered_map<int, MunicipioAfectado> tablaMunicipios;

    ColaPrioridad triajeMunicipios;
    DeliveryGraph redDeRutas;
    ArbolAVL arbolCaducidad;

    int totalLotesDespachados;
    float pesoTotalDespachado;

public:
    SistemaLogistico(const std::vector<Donante>& donantes, 
                     const std::vector<CentroAcopio>& centros,
                     const std::vector<LoteAyuda>& lotes,
                     const std::vector<MunicipioAfectado>& municipios);

    // Operaciones de inventario y Hash Maps (Fase 1 y 2)
    void registrarDonacionEntrante(const LoteAyuda& nuevoLote, float volumen = 1.5f, bool requiereFrio = false);
    void buscarSuministroPorId(int idLote);
    void generarInformeStock();
    void darDeBajaSuministro(int idLote);
    void filtrarSuministrosPorCategoria(const std::string& categoria);

    // Operaciones de Triaje con Max-Heap (Fase 3)
    void consultarProximoMunicipio();
    void atenderMunicipioMasUrgente();
    void actualizarPrioridadMunicipio(int idMunicipio, int nuevoNivel, int nuevasNecesidades);

    // Operaciones de Arbol Balanceado AVL por caducidad (Fase 3)
    void consultarRangoCaducidad(const std::string& fechaInicio, const std::string& fechaFin);
    void listarLotesPorVencimiento();

    // Operaciones de Grafos, Dijkstra y Asignacion Automatizada (Fase 2 y 3)
    void agregarRutaMapa(int idOrigen, int idDestino, float distancia);
    void imprimirMapaRutas();
    void despacharRutaOptima(int idCentro, int idMunicipio);
    void asignarRecursosAutomatizado(int idCentroOrigen);
    void despacharRutaMultipunto(int idCentroOrigen, const std::vector<int>& destinos);

    // Suite integral de Pruebas Unitarias y Benchmarking (Rubrica 20%)
    void ejecutarSuitePruebasYBenchmarking();
};

#endif