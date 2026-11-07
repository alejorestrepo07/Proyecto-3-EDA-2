#include "ColaPrioridad.h"
#include <stdexcept>
#include <algorithm>

// Constructor por defecto
ColaPrioridad::ColaPrioridad() {
}

// Compara dos municipios por nivel de urgencia y desempata por cantidad de necesidades
bool ColaPrioridad::tieneMayorPrioridad(const MunicipioAfectado& a, const MunicipioAfectado& b) {
    if (a.nivelUrgencia != b.nivelUrgencia) {
        return a.nivelUrgencia > b.nivelUrgencia;
    }
    return a.cantidadNecesidades > b.cantidadNecesidades;
}

// Reordena hacia arriba en O(log n)
void ColaPrioridad::flotar(int index) {
    // Declaracion de variables al inicio del bloque
    int parentIndex;

    while (index > 0) {
        parentIndex = (index - 1) / 2;
        if (tieneMayorPrioridad(heapVector[index], heapVector[parentIndex])) {
            std::swap(heapVector[index], heapVector[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

// Reordena hacia abajo en O(log n)
void ColaPrioridad::hundir(int index) {
    // Declaracion de variables al inicio del bloque
    int size;
    int leftChild;
    int rightChild;
    int largestIndex;

    size = static_cast<int>(heapVector.size());

    while (true) {
        leftChild = 2 * index + 1;
        rightChild = 2 * index + 2;
        largestIndex = index;

        if (leftChild < size && tieneMayorPrioridad(heapVector[leftChild], heapVector[largestIndex])) {
            largestIndex = leftChild;
        }
        if (rightChild < size && tieneMayorPrioridad(heapVector[rightChild], heapVector[largestIndex])) {
            largestIndex = rightChild;
        }
        if (largestIndex != index) {
            std::swap(heapVector[index], heapVector[largestIndex]);
            index = largestIndex;
        } else {
            break;
        }
    }
}

// Inserta un nuevo municipio en el Max-Heap en O(log n)
void ColaPrioridad::insertar(const MunicipioAfectado& municipio) {
    // Declaracion de variables al inicio del bloque
    int lastIndex;

    heapVector.push_back(municipio);
    lastIndex = static_cast<int>(heapVector.size()) - 1;
    flotar(lastIndex);
}

// Extrae y retorna el municipio con mayor urgencia en O(log n)
MunicipioAfectado ColaPrioridad::extraerMaximo() {
    // Declaracion de variables al inicio del bloque
    MunicipioAfectado maxElement;

    if (heapVector.empty()) {
        throw std::out_of_range("La cola de prioridad se encuentra vacia.");
    }

    maxElement = heapVector[0];
    heapVector[0] = heapVector.back();
    heapVector.pop_back();

    if (!heapVector.empty()) {
        hundir(0);
    }

    return maxElement;
}

// Verifica si el monticulo esta vacio en O(1)
bool ColaPrioridad::estaVacia() const {
    return heapVector.empty();
}

// Retorna la cantidad de elementos en el monticulo en O(1)
int ColaPrioridad::obtenerTamano() const {
    return static_cast<int>(heapVector.size());
}