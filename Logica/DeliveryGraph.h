#ifndef DELIVERY_GRAPH_H
#define DELIVERY_GRAPH_H

#include <unordered_map>
#include <vector>
#include <utility>

// Clase para modelar la red de rutas logisticas mediante un Grafo ponderado
class DeliveryGraph {
private:
    // Lista de adyacencia: mapea un ID de origen a un vector de pares (ID destino, distancia en km)
    std::unordered_map<int, std::vector<std::pair<int, float>>> adjacencyList;

public:
    // Constructor vacio
    DeliveryGraph();

    // Metodo para agregar una ruta (arista) entre dos ubicaciones (vertices)
    void addEdge(int sourceId, int destinationId, float distance);

    // Algoritmo de Dijkstra para calcular y mostrar la ruta optima (O(E log V))
    void calculateShortestPath(int startId, int targetId);

    // Metodo para imprimir todas las conexiones de la red (O(V+E))
    void printGraph();

    // Calcula una ruta multipunto ordenada visitando varios destinos en cadena
    void calculateMultiStopRoute(int startId, const std::vector<int>& targetIds);
};

#endif