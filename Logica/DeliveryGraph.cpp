#include "DeliveryGraph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

// Constructor vacio
DeliveryGraph::DeliveryGraph() {
}

// Metodo para conectar los puntos en el mapa (aristas bidireccionales)
void DeliveryGraph::addEdge(int sourceId, int destinationId, float distance) {
    // Declaracion de variables al inicio del bloque
    std::pair<int, float> forwardEdge;
    std::pair<int, float> backwardEdge;

    forwardEdge = std::make_pair(destinationId, distance);
    backwardEdge = std::make_pair(sourceId, distance);

    adjacencyList[sourceId].push_back(forwardEdge);
    adjacencyList[destinationId].push_back(backwardEdge);
}

// Algoritmo de Dijkstra para la ruta mas corta O(E log V)
void DeliveryGraph::calculateShortestPath(int startId, int targetId) {
    // Declaracion de todas las variables al inicio del bloque
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> minHeap;
    std::unordered_map<int, float> distances;
    std::unordered_map<int, int> predecessors;
    std::unordered_map<int, std::vector<std::pair<int, float>>>::iterator graphIt;
    std::vector<std::pair<int, float>>::iterator neighborIt;
    std::pair<float, int> currentTop;
    int currentNode;
    float currentDist;
    int neighborNode;
    float edgeWeight;
    std::vector<int> finalPath;
    int tempNode;
    size_t i;

    for (graphIt = adjacencyList.begin(); graphIt != adjacencyList.end(); ++graphIt) {
        distances[graphIt->first] = std::numeric_limits<float>::infinity();
    }

    distances[startId] = 0.0f;
    minHeap.push(std::make_pair(0.0f, startId));

    while (!minHeap.empty()) {
        currentTop = minHeap.top();
        minHeap.pop();

        currentDist = currentTop.first;
        currentNode = currentTop.second;

        if (currentNode == targetId) {
            break;
        }

        if (currentDist > distances[currentNode]) {
            continue;
        }

        for (neighborIt = adjacencyList[currentNode].begin(); neighborIt != adjacencyList[currentNode].end(); ++neighborIt) {
            neighborNode = neighborIt->first;
            edgeWeight = neighborIt->second;

            if (distances[currentNode] + edgeWeight < distances[neighborNode]) {
                distances[neighborNode] = distances[currentNode] + edgeWeight;
                predecessors[neighborNode] = currentNode;
                minHeap.push(std::make_pair(distances[neighborNode], neighborNode));
            }
        }
    }

    if (distances.find(targetId) == distances.end() || distances[targetId] == std::numeric_limits<float>::infinity()) {
        std::cout << "\033[1;31m[ALERTA]\033[0m No hay ruta disponible para llegar al destino " << targetId << ".\n";
        return;
    }

    std::cout << "\n\033[1;32m[RUTAS LOGISTICAS - DIJKSTRA]\033[0m\n";
    std::cout << "-> Distancia total optima: " << distances[targetId] << " km\n";
    std::cout << "-> Ruta logistica (IDs): ";

    tempNode = targetId;
    while (tempNode != startId) {
        finalPath.push_back(tempNode);
        tempNode = predecessors[tempNode];
    }
    finalPath.push_back(startId);

    std::reverse(finalPath.begin(), finalPath.end());

    for (i = 0; i < finalPath.size(); i++) {
        std::cout << finalPath[i];
        if (i < finalPath.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";
}

// Imprime el reporte de todas las carreteras habilitadas en el sistema
void DeliveryGraph::printGraph() {
    // Declaracion de variables al inicio del bloque
    std::unordered_map<int, std::vector<std::pair<int, float>>>::iterator graphIt;
    size_t i;

    std::cout << "\n\033[1;36m[MAPA LOGISTICO DE RUTAS ACTIVAS]\033[0m\n";
    for (graphIt = adjacencyList.begin(); graphIt != adjacencyList.end(); ++graphIt) {
        std::cout << "Ubicacion ID " << graphIt->first << " conecta con: ";
        for (i = 0; i < graphIt->second.size(); i++) {
            std::cout << "[ID " << graphIt->second[i].first << " a " << graphIt->second[i].second << " km] ";
        }
        std::cout << "\n";
    }
}

// Planificador de ruta multipunto ordenada (Dijkstra + Heuristica Vecino Mas Cercano)
void DeliveryGraph::calculateMultiStopRoute(int startId, const std::vector<int>& targetIds) {
    // Declaracion de todas las variables al inicio del bloque
    std::vector<std::pair<float, int>> initialRanking;
    std::vector<int> pendingTargets;
    std::vector<int> visitOrder;
    std::vector<int> fullRoadSequence;
    std::unordered_map<int, std::vector<std::pair<int, float>>>::iterator graphIt;
    std::vector<std::pair<int, float>>::iterator neighborIt;
    int currentOrigin;
    float totalTourDistance;
    size_t i;
    size_t j;
    int bestNextTarget;
    float bestNextDistance;
    int bestIndex;
    std::vector<int> bestSegmentPath;

    if (targetIds.empty()) {
        std::cout << "\033[1;33m[AVISO]\033[0m No se ingresaron municipios destino para la ruta.\n";
        return;
    }

    // FASE A: Calcular distancia inicial desde el Centro de Acopio para ordenar de menor a mayor
    for (i = 0; i < targetIds.size(); i++) {
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> minHeap;
        std::unordered_map<int, float> distances;
        std::pair<float, int> currentTop;
        int currentNode;
        float currentDist;
        int neighborNode;
        float edgeWeight;

        for (graphIt = adjacencyList.begin(); graphIt != adjacencyList.end(); ++graphIt) {
            distances[graphIt->first] = std::numeric_limits<float>::infinity();
        }
        distances[startId] = 0.0f;
        minHeap.push(std::make_pair(0.0f, startId));

        while (!minHeap.empty()) {
            currentTop = minHeap.top();
            minHeap.pop();
            currentDist = currentTop.first;
            currentNode = currentTop.second;

            if (currentNode == targetIds[i]) {
                break;
            }
            if (currentDist > distances[currentNode]) {
                continue;
            }
            for (neighborIt = adjacencyList[currentNode].begin(); neighborIt != adjacencyList[currentNode].end(); ++neighborIt) {
                neighborNode = neighborIt->first;
                edgeWeight = neighborIt->second;
                if (distances[currentNode] + edgeWeight < distances[neighborNode]) {
                    distances[neighborNode] = distances[currentNode] + edgeWeight;
                    minHeap.push(std::make_pair(distances[neighborNode], neighborNode));
                }
            }
        }

        if (distances.find(targetIds[i]) != distances.end() && distances[targetIds[i]] != std::numeric_limits<float>::infinity()) {
            initialRanking.push_back(std::make_pair(distances[targetIds[i]], targetIds[i]));
            pendingTargets.push_back(targetIds[i]);
        } else {
            std::cout << "\033[1;31m[ALERTA]\033[0m El destino ID " << targetIds[i] << " no tiene conexion vial y sera omitido.\n";
        }
    }

    if (pendingTargets.empty()) {
        std::cout << "\033[1;31m[ERROR]\033[0m Ninguno de los destinos ingresados es alcanzable desde el origen " << startId << ".\n";
        return;
    }

    // Ordenamiento ascendente por distancia desde el punto de partida (del mas cercano al mas lejano)
    std::sort(initialRanking.begin(), initialRanking.end());

    std::cout << "\n\033[1;36m=== 1. CLASIFICACION DE MUNICIPIOS (DEL MAS CERCANO AL MAS LEJANO DESDE ID " << startId << ") ===\033[0m\n";
    for (i = 0; i < initialRanking.size(); i++) {
        std::cout << "   Posicion #" << (i + 1) << ": Municipio ID " << initialRanking[i].second 
                  << " (Distancia desde origen: " << initialRanking[i].first << " km)"
                  << (i == 0 ? " <-- [MAS CERCANO]" : (i == initialRanking.size() - 1 ? " <-- [MAS LEJANO]" : "")) << "\n";
    }

    // FASE B: Armar el recorrido encadenado saltando siempre al siguiente municipio mas cercano
    currentOrigin = startId;
    totalTourDistance = 0.0f;
    fullRoadSequence.push_back(startId);

    std::cout << "\n\033[1;32m=== 2. EJECUCION DE RUTA OPTIMA MULTIPARADA (TRAMO A TRAMO) ===\033[0m\n";

    while (!pendingTargets.empty()) {
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> stepHeap;
        std::unordered_map<int, float> stepDistances;
        std::unordered_map<int, int> stepPredecessors;
        std::pair<float, int> topElement;
        int uNode;
        float uDist;
        int vNode;
        float weight;
        int traceNode;

        for (graphIt = adjacencyList.begin(); graphIt != adjacencyList.end(); ++graphIt) {
            stepDistances[graphIt->first] = std::numeric_limits<float>::infinity();
        }
        stepDistances[currentOrigin] = 0.0f;
        stepHeap.push(std::make_pair(0.0f, currentOrigin));

        while (!stepHeap.empty()) {
            topElement = stepHeap.top();
            stepHeap.pop();
            uDist = topElement.first;
            uNode = topElement.second;

            if (uDist > stepDistances[uNode]) {
                continue;
            }
            for (neighborIt = adjacencyList[uNode].begin(); neighborIt != adjacencyList[uNode].end(); ++neighborIt) {
                vNode = neighborIt->first;
                weight = neighborIt->second;
                if (stepDistances[uNode] + weight < stepDistances[vNode]) {
                    stepDistances[vNode] = stepDistances[uNode] + weight;
                    stepPredecessors[vNode] = uNode;
                    stepHeap.push(std::make_pair(stepDistances[vNode], vNode));
                }
            }
        }

        bestNextTarget = -1;
        bestNextDistance = std::numeric_limits<float>::infinity();
        bestIndex = -1;

        for (j = 0; j < pendingTargets.size(); j++) {
            if (stepDistances[pendingTargets[j]] < bestNextDistance) {
                bestNextDistance = stepDistances[pendingTargets[j]];
                bestNextTarget = pendingTargets[j];
                bestIndex = static_cast<int>(j);
            }
        }

        if (bestNextTarget == -1) {
            break;
        }

        bestSegmentPath.clear();
        traceNode = bestNextTarget;
        while (traceNode != currentOrigin) {
            bestSegmentPath.push_back(traceNode);
            traceNode = stepPredecessors[traceNode];
        }
        std::reverse(bestSegmentPath.begin(), bestSegmentPath.end());

        for (j = 0; j < bestSegmentPath.size(); j++) {
            fullRoadSequence.push_back(bestSegmentPath[j]);
        }

        visitOrder.push_back(bestNextTarget);
        totalTourDistance += bestNextDistance;

        std::cout << "-> Tramo [Sale de ID " << currentOrigin << " -> Llega a Municipio ID " << bestNextTarget 
                  << "] | Recorrido del tramo: " << bestNextDistance << " km\n";

        currentOrigin = bestNextTarget;
        pendingTargets.erase(pendingTargets.begin() + bestIndex);
    }

    std::cout << "\n\033[1;32m[RESUMEN FINAL DE LA RUTA MULTIPUNTO]\033[0m\n";
    std::cout << "-> Secuencia ordenada de entregas: ";
    for (i = 0; i < visitOrder.size(); i++) {
        std::cout << visitOrder[i] << (i < visitOrder.size() - 1 ? " -> " : "\n");
    }

    std::cout << "-> Trazado completo por carretera (Nodos): ";
    for (i = 0; i < fullRoadSequence.size(); i++) {
        std::cout << fullRoadSequence[i] << (i < fullRoadSequence.size() - 1 ? " -> " : "\n");
    }
    std::cout << "-> Kilometraje total acumulado del recorrido: " << totalTourDistance << " km\n";
}