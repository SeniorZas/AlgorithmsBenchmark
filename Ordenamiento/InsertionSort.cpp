#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Implementación del algoritmo de Insertion Sort
// Este algoritmo ordena un vector de enteros.
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Mueve los elementos de arr[0..i-1], que son mayores que la clave, una posición adelante de su posición actual
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int promedio = 0;  // Variable para almacenar el tiempo total acumulado

    // Repetir 10 veces para obtener un promedio de tiempos
    for (int conta = 0; conta < 10; conta++) {
        
        // Abrir el archivo de entrada (datasetOrdenado.txt) y de salida (OrdenadasInsertionSort.txt)
        ifstream infile("datasetOrdenado.txt");
        ofstream outfile("OrdenadasInsertionSort.txt");

        // Comprobar si el archivo de entrada se abrió correctamente
        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo para leer." << endl;
            return 1;
        }

        // Comprobar si el archivo de salida se abrió correctamente
        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo para escribir." << endl;
            return 1;
        }

        std::chrono::duration<double, milli> duracion;  // Variable para medir la duración
        string line;

        // Leer el archivo de entrada línea por línea
        while (getline(infile, line)) {
            vector<int> lista;  // Vector para almacenar los números de la línea
            istringstream ss(line);
            int number;

            // Lee los números del archivo y los agrega al vector
            while (ss >> number) {
                lista.push_back(number);
            }

            // Comienza el cronometraje antes de la ordenación
            auto start = high_resolution_clock::now();
            
            // Llama a la función de ordenación Insertion Sort
            insertionSort(lista);

            // Termina el cronometraje después de la ordenación
            auto end = high_resolution_clock::now();

            // Calcula la duración de la ordenación
            duracion = end - start;

            // Escribe la lista ordenada en el archivo de salida
            for (const auto& elemento : lista) {
                outfile << elemento << " ";
            }
        }

        // Muestra el tiempo de ejecución para cada iteración
        cout <<"Tiempo transcurrido: " << duracion.count() << " ms" << endl;
        
        // Acumula el tiempo de ejecución en la variable 'promedio'
        promedio += duracion.count();

        // Cierra los archivos después de cada iteración
        infile.close();
        outfile.close();
    }

    // Calcula y muestra el tiempo promedio de ejecución
    cout << "Tiempo promedio: " << promedio / 10 << " ms" << endl;

    return 0;
}
