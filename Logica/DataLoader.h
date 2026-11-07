#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <vector>
#include <string>
#include "../Modelos/Modelos.h"

// Clase encargada de la lectura, parsing y validacion de archivos CSV (Fase 1)
class DataLoader {
public:
    static std::vector<Donante> cargarDonantes(const std::string& filePath);
    static std::vector<LoteAyuda> cargarLotes(const std::string& filePath);
    static std::vector<CentroAcopio> cargarCentros(const std::string& filePath);
    static std::vector<MunicipioAfectado> cargarMunicipios(const std::string& filePath);
};

#endif