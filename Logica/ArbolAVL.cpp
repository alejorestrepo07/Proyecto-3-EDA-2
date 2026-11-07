#include "ArbolAVL.h"
#include <iostream>

// Constructor del Arbol AVL
ArbolAVL::ArbolAVL() {
    root = nullptr;
}

// Destructor para liberar memoria dinamica
ArbolAVL::~ArbolAVL() {
    destroyTree(root);
}

// Libera recursivamente los nodos del arbol
void ArbolAVL::destroyTree(AVLNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Obtiene la altura de un nodo en O(1)
int ArbolAVL::getHeight(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Calcula el maximo entre dos enteros
int ArbolAVL::maxInt(int a, int b) {
    return (a > b) ? a : b;
}

// Obtiene el factor de equilibrio del nodo
int ArbolAVL::getBalanceFactor(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Rotacion simple a la derecha para balanceo AVL en O(1)
AVLNode* ArbolAVL::rotateRight(AVLNode* y) {
    // Declaracion de variables al inicio del bloque
    AVLNode* x;
    AVLNode* t2;

    x = y->left;
    t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = maxInt(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxInt(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Rotacion simple a la izquierda para balanceo AVL en O(1)
AVLNode* ArbolAVL::rotateLeft(AVLNode* x) {
    // Declaracion de variables al inicio del bloque
    AVLNode* y;
    AVLNode* t2;

    y = x->right;
    t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = maxInt(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = maxInt(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Normaliza fechas en formato DD/MM/AAAA o AAAA-MM-DD a clave numerica AAAAMMDD
int ArbolAVL::convertDateToKey(const std::string& dateStr) {
    // Declaracion de variables al inicio del bloque
    int year;
    int month;
    int day;

    year = 2099;
    month = 12;
    day = 31;

    try {
        if (dateStr.length() >= 10) {
            if (dateStr[4] == '-' || dateStr[4] == '/') {
                year = std::stoi(dateStr.substr(0, 4));
                month = std::stoi(dateStr.substr(5, 2));
                day = std::stoi(dateStr.substr(8, 2));
            } else if (dateStr[2] == '/' || dateStr[2] == '-') {
                day = std::stoi(dateStr.substr(0, 2));
                month = std::stoi(dateStr.substr(3, 2));
                year = std::stoi(dateStr.substr(6, 4));
            }
        }
    } catch (...) {
        year = 2099;
        month = 12;
        day = 31;
    }

    return (year * 10000) + (month * 100) + day;
}

// Insercion recursiva con autobalanceo AVL en O(log n)
AVLNode* ArbolAVL::insertNode(AVLNode* node, int dateKey, const LoteAyuda& batch) {
    // Declaracion de variables al inicio del bloque
    AVLNode* newNode;
    int balance;

    if (node == nullptr) {
        newNode = new AVLNode();
        newNode->dateKey = dateKey;
        newNode->dateString = batch.fechaCaducidad;
        newNode->batches.push_back(batch);
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (dateKey < node->dateKey) {
        node->left = insertNode(node->left, dateKey, batch);
    } else if (dateKey > node->dateKey) {
        node->right = insertNode(node->right, dateKey, batch);
    } else {
        node->batches.push_back(batch);
        return node;
    }

    node->height = 1 + maxInt(getHeight(node->left), getHeight(node->right));
    balance = getBalanceFactor(node);

    // Caso Izquierda-Izquierda
    if (balance > 1 && dateKey < node->left->dateKey) {
        return rotateRight(node);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && dateKey > node->right->dateKey) {
        return rotateLeft(node);
    }

    // Caso Izquierda-Derecha
    if (balance > 1 && dateKey > node->left->dateKey) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && dateKey < node->right->dateKey) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Metodo publico para insertar un lote en el AVL
void ArbolAVL::insertBatch(const LoteAyuda& batch) {
    // Declaracion de variables al inicio del bloque
    int key;

    key = convertDateToKey(batch.fechaCaducidad);
    root = insertNode(root, key, batch);
}

// Remueve un lote especifico de los vectores del arbol cuando se despacha
void ArbolAVL::removeBatchById(AVLNode* node, int idLote) {
    // Declaracion de variables al inicio del bloque
    size_t i;

    if (node == nullptr) {
        return;
    }

    for (i = 0; i < node->batches.size(); i++) {
        if (node->batches[i].idLote == idLote) {
            node->batches.erase(node->batches.begin() + i);
            break;
        }
    }

    removeBatchById(node->left, idLote);
    removeBatchById(node->right, idLote);
}

// Metodo publico para remover lote por ID
void ArbolAVL::removeBatch(int idLote) {
    removeBatchById(root, idLote);
}

// Recorrido acotado para consulta de rango en O(log n + k)
void ArbolAVL::rangeQueryHelper(AVLNode* node, int startKey, int endKey, int& foundCount) {
    // Declaracion de variables al inicio del bloque
    size_t i;

    if (node == nullptr) {
        return;
    }

    if (startKey < node->dateKey) {
        rangeQueryHelper(node->left, startKey, endKey, foundCount);
    }

    if (startKey <= node->dateKey && endKey >= node->dateKey) {
        for (i = 0; i < node->batches.size(); i++) {
            std::cout << "   -> Lote ID: " << node->batches[i].idLote
                      << " | Tipo: " << node->batches[i].tipoSuministro
                      << " | Peso: " << node->batches[i].peso << " kg"
                      << " | Expira: " << node->batches[i].fechaCaducidad << "\n";
            foundCount++;
        }
    }

    if (endKey > node->dateKey) {
        rangeQueryHelper(node->right, startKey, endKey, foundCount);
    }
}

// Metodo publico para consultar suministros por rango de vencimiento
void ArbolAVL::searchByDateRange(const std::string& startDate, const std::string& endDate) {
    // Declaracion de variables al inicio del bloque
    int startKey;
    int endKey;
    int foundCount;
    int tempKey;

    startKey = convertDateToKey(startDate);
    endKey = convertDateToKey(endDate);
    foundCount = 0;

    if (startKey > endKey) {
        tempKey = startKey;
        startKey = endKey;
        endKey = tempKey;
    }

    std::cout << "\n\033[1;36m[ARBOL AVL - CONSULTA DE RANGO DE CADUCIDAD O(log n)]\033[0m\n";
    std::cout << "Rango evaluado: " << startDate << " hasta " << endDate << "\n";

    rangeQueryHelper(root, startKey, endKey, foundCount);

    if (foundCount == 0) {
        std::cout << "\033[1;33m[INFO]\033[0m No se encontraron suministros que venzan en ese rango de fechas.\n";
    } else {
        std::cout << "\033[1;32mTotal de lotes en rango:\033[0m " << foundCount << "\n";
    }
}

// Recorrido In-Order para mostrar lotes ordenados por fecha de vencimiento
void ArbolAVL::inOrderHelper(AVLNode* node) {
    // Declaracion de variables al inicio del bloque
    size_t i;

    if (node == nullptr) {
        return;
    }

    inOrderHelper(node->left);
    for (i = 0; i < node->batches.size(); i++) {
        std::cout << "   [Vence: " << node->batches[i].fechaCaducidad << "] Lote ID: "
                  << node->batches[i].idLote << " | Tipo: " << node->batches[i].tipoSuministro
                  << " | Peso: " << node->batches[i].peso << " kg\n";
    }
    inOrderHelper(node->right);
}

// Imprime todos los suministros en orden cronologico de caducidad
void ArbolAVL::printAllByExpiry() {
    std::cout << "\n\033[1;36m[ARBOL AVL - LOTES ORDENADOS POR PROXIMIDAD DE VENCIMIENTO]\033[0m\n";
    if (root == nullptr) {
        std::cout << "No hay suministros registrados en el arbol.\n";
        return;
    }
    inOrderHelper(root);
}