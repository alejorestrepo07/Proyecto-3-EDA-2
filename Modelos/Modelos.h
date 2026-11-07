#ifndef MODELOS_H
#define MODELOS_H

#include <string>

// Entidad Donante
struct Donante {
    int idDonante;
    std::string nombre;
    std::string tipoDonante;
};

// Entidad Lote de Ayuda Humanitaria
struct LoteAyuda {
    int idLote;
    std::string tipoSuministro;
    float peso;
    std::string fechaCaducidad;
};

// Entidad Centro de Acopio Regional
struct CentroAcopio {
    int idCentro;
    std::string nombre;
    std::string ubicacion;
    float capacidadMaxima;
};

// Entidad Municipio Afectado
struct MunicipioAfectado {
    int idMunicipio;
    std::string nombre;
    int nivelUrgencia;
    int cantidadNecesidades;
};

#endif