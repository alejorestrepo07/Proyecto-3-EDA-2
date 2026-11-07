#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include <string>
#include <vector>
#include "../Modelos/Modelos.h"

// Nodo del Arbol Binario de Busqueda Balanceado (AVL) indexado por fecha de caducidad
struct AVLNode {
    int dateKey;
    std::string dateString;
    std::vector<LoteAyuda> batches;
    AVLNode* left;
    AVLNode* right;
    int height;
};

// Clase ArbolAVL para gestion ordenada y consultas de rango en O(log n)
class ArbolAVL {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node);
    int maxInt(int a, int b);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, int dateKey, const LoteAyuda& batch);
    void rangeQueryHelper(AVLNode* node, int startKey, int endKey, int& foundCount);
    void inOrderHelper(AVLNode* node);
    void destroyTree(AVLNode* node);

public:
    ArbolAVL();
    ~ArbolAVL();

    // Convierte fechas DD/MM/AAAA o AAAA-MM-DD a entero AAAAMMDD para comparacion exacta
    int convertDateToKey(const std::string& dateStr);

    // Inserta un lote en el arbol manteniendo el balance O(log n)
    void insertBatch(const LoteAyuda& batch);

    // Elimina un lote especifico cuando es despachado
    void removeBatchById(AVLNode* node, int idLote);
    void removeBatch(int idLote);

    // Consulta de lotes por rango de fechas de expiracion en O(log n + k)
    void searchByDateRange(const std::string& startDate, const std::string& endDate);

    // Muestra todos los lotes ordenados cronologicamente por vencimiento
    void printAllByExpiry();
};

#endif