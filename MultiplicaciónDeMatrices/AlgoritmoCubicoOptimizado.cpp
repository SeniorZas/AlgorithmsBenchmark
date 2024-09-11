#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para multiplicar dos matrices con optimización básica
void multiplicarMatricesOptimizado(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();  // Número de filas de A
    int m = A[0].size();  // Número de columnas de A y filas de B
    int p = B[0].size();  // Número de columnas de B

    // Inicializar la matriz de resultado C con ceros
    C.assign(n, vector<int>(p, 0));

    // Optimización básica para evitar múltiples accesos a la misma posición en C
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < m; ++k) {
            int temp = A[i][k];  // Guardamos el valor de A[i][k] para reducir accesos a memoria
            for (int j = 0; j < p; ++j) {
                C[i][j] += temp * B[k][j];  // Realizamos la multiplicación
            }
        }
    }
}

int main() {
    // Realizar 3 iteraciones para medir tiempos múltiples
    for (int conta = 0; conta < 3; conta++) {
        ifstream infile("inputscubicos.txt");  // Archivo de entrada con las matrices
        ofstream outfile2("TiemposCubicoOptimizado.csv", ios::app);  // Archivo para almacenar los tiempos

        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo 'inputscubicos.txt' para leer." << endl;
            return 1;
        }

        ofstream outfile("resultadoscuboptimizado.txt");  // Archivo de salida para los resultados
        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo 'resultadoscuboptimizado.txt' para escribir." << endl;
            return 1;
        }

        vector<vector<vector<int>>> matrices;  // Vector 3D para almacenar todas las matrices
        vector<pair<int, int>> sizes;  // Vector para almacenar las dimensiones (filas, columnas) de cada matriz
        int rows, cols;

        // Leer matrices del archivo de entrada
        while (infile >> rows >> cols) {
            vector<vector<int>> matriz(rows, vector<int>(cols));
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    infile >> matriz[i][j];  // Leer cada valor de la matriz
                }
            }
            matrices.push_back(matriz);  // Añadir matriz al vector de matrices
            sizes.push_back({rows, cols});  // Guardar las dimensiones
        }

        infile.close();  // Cerrar el archivo de entrada

        if (matrices.size() % 2 != 0) {
            cerr << "El número de matrices no es par." << endl;
            return 1;
        }

        // Medir el tiempo total de procesamiento
        auto start_total = high_resolution_clock::now();

        // Multiplicar las matrices de dos en dos
        for (size_t i = 0; i < matrices.size(); i += 2) {
            int filasA = sizes[i].first;
            int columnasA = sizes[i].second;
            int filasB = sizes[i + 1].first;
            int columnasB = sizes[i + 1].second;

            // Verificar que las matrices sean compatibles para la multiplicación
            if (columnasA != filasB) {
                cerr << "Las dimensiones de las matrices no son compatibles para la multiplicación: "
                     << columnasA << " columnas en la primera y " << filasB << " filas en la segunda." << endl;
                return 1;
            }

            // Matriz para almacenar el resultado de la multiplicación
            vector<vector<int>> resultado;
            multiplicarMatricesOptimizado(matrices[i], matrices[i + 1], resultado);

            // Escribir las matrices y el resultado en el archivo de salida
            outfile << "Matriz A (" << filasA << "x" << columnasA << "):" << endl;
            for (const auto& fila : matrices[i]) {
                for (int num : fila) {
                    outfile << num << " ";
                }
                outfile << endl;
            }

            outfile << endl << "Matriz B (" << filasB << "x" << columnasB << "):" << endl;
            for (const auto& fila : matrices[i + 1]) {
                for (int num : fila) {
                    outfile << num << " ";
                }
                outfile << endl;
            }

            outfile << endl << "Resultado de la multiplicación (" << filasA << "x" << columnasB << "):" << endl;
            for (const auto& fila : resultado) {
                for (int num : fila) {
                    outfile << num << " ";
                }
                outfile << endl;
            }

            outfile << endl;  // Separar los resultados con líneas en blanco
        }

        // Medir el tiempo total de procesamiento
        auto end_total = high_resolution_clock::now();
        auto duration_total = duration_cast<milliseconds>(end_total - start_total);

        outfile.close();  // Cerrar el archivo de salida

        // Imprimir y almacenar el tiempo total de procesamiento
        cout << "La multiplicación de matrices ha sido escrita en 'resultadoscuboptimizado.txt'." << endl;
        cout << "Tiempo total de procesamiento: " << duration_total.count() << " milisegundos" << endl;
        outfile2 << duration_total.count() << endl;
        outfile2.close();
    }

    return 0;
}
