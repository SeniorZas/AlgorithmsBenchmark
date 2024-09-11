#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para multiplicar dos matrices
void multiplicarMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size(); // Número de filas de A
    int m = A[0].size(); // Número de columnas de A (y filas de B)
    int p = B[0].size(); // Número de columnas de B

    // Inicializar la matriz de resultado C con ceros
    C.assign(n, vector<int>(p, 0));

    // Multiplicación de matrices: clásico O(n^3)
    for (int i = 0; i < n; ++i) {      // Recorre las filas de la matriz A
        for (int j = 0; j < p; ++j) {  // Recorre las columnas de la matriz B
            for (int k = 0; k < m; ++k) { // Multiplica y acumula los valores
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Abrimos el archivo de entrada para leer las matrices
    ifstream infile("Input.txt");  
    ofstream outfile2("TiemposCubicoTradicional.csv", ios::app); // Para registrar los tiempos de ejecución

    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo 'Input.txt' para leer." << endl;
        return 1;
    }

    // Abrimos el archivo de salida para guardar los resultados
    ofstream outfile("resultadoscubtradicional.txt"); 
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo 'resultadoscubtradicional.txt' para escribir." << endl;
        return 1;
    }

    vector<vector<vector<int>>> matrices;  // Vector para almacenar todas las matrices leídas
    vector<pair<int, int>> sizes;          // Almacena las dimensiones de las matrices (filas, columnas)
    int rows, cols;

    // Leer todas las matrices desde el archivo
    while (infile >> rows >> cols) {  // Se espera que primero vengan las dimensiones de cada matriz
        vector<vector<int>> matriz(rows, vector<int>(cols)); // Crear matriz con las dimensiones leídas
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                infile >> matriz[i][j];  // Leer cada valor de la matriz
            }
        }
        matrices.push_back(matriz);  // Guardar la matriz leída
        sizes.push_back({rows, cols});  // Guardar las dimensiones de la matriz
    }
    infile.close(); // Cerrar el archivo de entrada

    for(int conta = 0; conta < 3; conta++) {  // Bucle para realizar 3 ejecuciones del experimento

        // Iniciar medición del tiempo total de ejecución
        auto start_total = high_resolution_clock::now();
        
        // Recorremos el vector de matrices de dos en dos para multiplicarlas
        for (size_t i = 0; i < matrices.size(); i += 2) {
            int filasA = sizes[i].first;   // Número de filas de la matriz A
            int columnasA = sizes[i].second; // Número de columnas de la matriz A
            int filasB = sizes[i + 1].first; // Número de filas de la matriz B
            int columnasB = sizes[i + 1].second; // Número de columnas de la matriz B

            // Verificar si las dimensiones de las matrices son compatibles para la multiplicación
            if (columnasA != filasB) {
                cerr << "Dimensiones incompatibles para la multiplicación." << endl;
                return 1;
            }

            vector<vector<int>> resultado; // Matriz para guardar el resultado
            multiplicarMatrices(matrices[i], matrices[i + 1], resultado); // Realizar la multiplicación

            // Guardar el resultado de la multiplicación en el archivo de salida
            outfile << "Resultado " << conta + 1 << " de la multiplicación: " << endl;
            for (const auto& fila : resultado) {  // Iteramos sobre las filas del resultado
                for (int num : fila) {            // Escribimos cada número en la fila
                    outfile << num << " ";
                }
                outfile << endl;  // Nueva línea después de cada fila
            }
            outfile << endl;  // Espacio entre resultados
        }

        // Terminar la medición de tiempo y calcular la duración total
        auto end_total = high_resolution_clock::now();
        auto duration_total = duration_cast<milliseconds>(end_total - start_total);
        cout << "Tiempo: " << duration_total.count() << " ms" << endl; // Mostrar el tiempo en consola
        outfile2 << duration_total.count() << endl;  // Guardar el tiempo en el archivo CSV
    }

    outfile.close(); // Cerrar el archivo de resultados
    outfile2.close(); // Cerrar el archivo de tiempos

    return 0;
}
