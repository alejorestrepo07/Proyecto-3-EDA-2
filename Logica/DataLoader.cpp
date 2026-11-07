#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Carga y valida registros de donantes desde archivo CSV
std::vector<Donante> DataLoader::cargarDonantes(const std::string& filePath) {
    // Declaracion de variables al inicio del bloque
    std::vector<Donante> donorsList;
    std::ifstream fileStream(filePath);
    std::string currentLine;
    std::string idStr;
    std::string nameStr;
    std::string typeStr;
    Donante tempDonor;

    if (!fileStream.is_open()) {
        std::cout << "[AVISO I/O] No se pudo abrir " << filePath << ". Operando con datos en memoria.\n";
        return donorsList;
    }

    std::getline(fileStream, currentLine); // Omitir cabecera

    while (std::getline(fileStream, currentLine)) {
        if (currentLine.empty()) {
            continue;
        }
        std::stringstream lineStream(currentLine);
        if (std::getline(lineStream, idStr, ',') &&
            std::getline(lineStream, nameStr, ',') &&
            std::getline(lineStream, typeStr, ',')) {
            try {
                tempDonor.idDonante = std::stoi(idStr);
                tempDonor.nombre = nameStr;
                tempDonor.tipoDonante = typeStr;
                if (tempDonor.idDonante > 0 && !tempDonor.nombre.empty()) {
                    donorsList.push_back(tempDonor);
                }
            } catch (...) {
                std::cout << "Registro de donante invalido omitido por seguridad\n";
            }
        }
    }
    fileStream.close();
    return donorsList;
}

// Carga y valida lotes de suministros verificando pesos positivos y formato
std::vector<LoteAyuda> DataLoader::cargarLotes(const std::string& filePath) {
    // Declaracion de variables al inicio del bloque
    std::vector<LoteAyuda> batchesList;
    std::ifstream fileStream(filePath);
    std::string currentLine;
    std::string idStr;
    std::string typeStr;
    std::string weightStr;
    std::string expiryStr;
    LoteAyuda tempBatch;

    if (!fileStream.is_open()) {
        std::cout << "[AVISO I/O] No se pudo abrir " << filePath << ". Operando con datos en memoria.\n";
        return batchesList;
    }

    std::getline(fileStream, currentLine); // Omitir cabecera

    while (std::getline(fileStream, currentLine)) {
        if (currentLine.empty()) {
            continue;
        }
        std::stringstream lineStream(currentLine);
        if (std::getline(lineStream, idStr, ',') &&
            std::getline(lineStream, typeStr, ',') &&
            std::getline(lineStream, weightStr, ',') &&
            std::getline(lineStream, expiryStr, ',')) {
            try {
                tempBatch.idLote = std::stoi(idStr);
                tempBatch.tipoSuministro = typeStr;
                tempBatch.peso = std::stof(weightStr);
                tempBatch.fechaCaducidad = expiryStr;

                if (tempBatch.idLote > 0 && tempBatch.peso > 0.0f && !tempBatch.fechaCaducidad.empty()) {
                    batchesList.push_back(tempBatch);
                } else {
                    std::cout << "Registro de lote invalido omitido por seguridad\n";
                }
            } catch (...) {
                std::cout << "Registro de lote invalido omitido por seguridad\n";
            }
        }
    }
    fileStream.close();
    return batchesList;
}

// Carga y valida centros de acopio regionales
std::vector<CentroAcopio> DataLoader::cargarCentros(const std::string& filePath) {
    // Declaracion de variables al inicio del bloque
    std::vector<CentroAcopio> centersList;
    std::ifstream fileStream(filePath);
    std::string currentLine;
    std::string idStr;
    std::string nameStr;
    std::string locStr;
    std::string capStr;
    CentroAcopio tempCenter;

    if (!fileStream.is_open()) {
        std::cout << "[AVISO I/O] No se pudo abrir " << filePath << ". Operando con datos en memoria.\n";
        return centersList;
    }

    std::getline(fileStream, currentLine); // Omitir cabecera

    while (std::getline(fileStream, currentLine)) {
        if (currentLine.empty()) {
            continue;
        }
        std::stringstream lineStream(currentLine);
        if (std::getline(lineStream, idStr, ',') &&
            std::getline(lineStream, nameStr, ',') &&
            std::getline(lineStream, locStr, ',') &&
            std::getline(lineStream, capStr, ',')) {
            try {
                tempCenter.idCentro = std::stoi(idStr);
                tempCenter.nombre = nameStr;
                tempCenter.ubicacion = locStr;
                tempCenter.capacidadMaxima = std::stof(capStr);

                if (tempCenter.idCentro > 0 && tempCenter.capacidadMaxima > 0.0f) {
                    centersList.push_back(tempCenter);
                } else {
                    std::cout << "Fallo al leer registro de centro de acopio\n";
                }
            } catch (...) {
                std::cout << "Fallo al leer registro de centro de acopio\n";
            }
        }
    }
    fileStream.close();
    return centersList;
}

// Carga y valida municipios afectados verificando rangos de urgencia (1 a 5)
std::vector<MunicipioAfectado> DataLoader::cargarMunicipios(const std::string& filePath) {
    // Declaracion de variables al inicio del bloque
    std::vector<MunicipioAfectado> municipalitiesList;
    std::ifstream fileStream(filePath);
    std::string currentLine;
    std::string idStr;
    std::string nameStr;
    std::string urgencyStr;
    std::string needsStr;
    MunicipioAfectado tempMunicipality;

    if (!fileStream.is_open()) {
        std::cout << "[AVISO I/O] No se pudo abrir " << filePath << ". Operando con datos en memoria.\n";
        return municipalitiesList;
    }

    std::getline(fileStream, currentLine); // Omitir cabecera

    while (std::getline(fileStream, currentLine)) {
        if (currentLine.empty()) {
            continue;
        }
        std::stringstream lineStream(currentLine);
        if (std::getline(lineStream, idStr, ',') &&
            std::getline(lineStream, nameStr, ',') &&
            std::getline(lineStream, urgencyStr, ',') &&
            std::getline(lineStream, needsStr, ',')) {
            try {
                tempMunicipality.idMunicipio = std::stoi(idStr);
                tempMunicipality.nombre = nameStr;
                tempMunicipality.nivelUrgencia = std::stoi(urgencyStr);
                tempMunicipality.cantidadNecesidades = std::stoi(needsStr);

                if (tempMunicipality.idMunicipio > 0 &&
                    tempMunicipality.nivelUrgencia >= 1 && tempMunicipality.nivelUrgencia <= 5 &&
                    tempMunicipality.cantidadNecesidades >= 0) {
                    municipalitiesList.push_back(tempMunicipality);
                } else {
                    std::cout << "Registro de municipio ignorado por errores de formato\n";
                }
            } catch (...) {
                std::cout << "Registro de municipio ignorado por errores de formato\n";
            }
        }
    }
    fileStream.close();
    return municipalitiesList;
}