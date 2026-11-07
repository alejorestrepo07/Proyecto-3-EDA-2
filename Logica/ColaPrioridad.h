#ifndef COLA_PRIORIDAD_H
#define COLA_PRIORIDAD_H

#include <vector>
#include "../Modelos/Modelos.h"

// Implementacion de Monticulo de Maximos (Max-Heap) para triaje municipal O(log n)
class ColaPrioridad {
private:
    std::vector<MunicipioAfectado> heapVector;

    void flotar(int index);
    void hundir(int index);
    bool tieneMayorPrioridad(const MunicipioAfectado& a, const MunicipioAfectado& b);

public:
    ColaPrioridad();
    void insertar(const MunicipioAfectado& municipio);
    MunicipioAfectado extraerMaximo();
    bool estaVacia() const;
    int obtenerTamano() const;
};

#endif