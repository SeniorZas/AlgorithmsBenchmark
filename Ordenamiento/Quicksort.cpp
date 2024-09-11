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

// Función para encontrar la mediana de tres elementos
int medianOfThree(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;  // Calcula el índice del medio
    if (arr[low] > arr[mid])
        swap(arr[low], arr[mid]);  // Asegurar que arr[low] <= arr[mid]
    if (arr[low] > arr[high])
        swap(arr[low], arr[high]);  // Asegurar que arr[low] <= arr[high]
    if (arr[mid] > arr[high])
        swap(arr[mid], arr[high]);  // Asegurar que arr[mid] <= arr[high]
    return mid;  // Retorna el índice del pivote (mediana)
}

// Función de partición que utiliza la mediana de tres como pivote
int partition(vector<int>& arr, int low, int high) {
    // Usar la mediana de tres para seleccionar el pivote
    int median = medianOfThree(arr, low, high);
    swap(arr[median], arr[high]);  // Mover el pivote al final

    int pivot = arr[high];  // Tomar el pivote
    int i = low - 1;  // Índice del elemento más pequeño

    // Ordenar los elementos alrededor del pivote
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {  // Si el elemento actual es menor que el pivote
            i++;  // Incrementar el índice del elemento más pequeño
            swap(arr[i], arr[j]);  // Colocar el elemento en su lugar correcto
        }
    }
    swap(arr[i + 1], arr[high]);  // Colocar el pivote en su posición correcta
    return i + 1;  // Retornar el índice del pivote
}

// Implementación de Quicksort
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // Particionar el array y obtener el pivote

        quickSort(arr, low, pi - 1);  // Ordenar los elementos a la izquierda del pivote
        quickSort(arr, pi + 1, high);  // Ordenar los elementos a la derecha del pivote
    }
}

int main() {
    int promedio = 0;  // Para calcular el tiempo promedio de ejecución

    // Ejecutar el proceso 10 veces para calcular el promedio
    for (int conta = 0; conta < 10; conta++) {
        
        // Abrir los archivos de entrada y salida
        ifstream infile("datasets/datasetMitadOrdenado10000.txt");
        ofstream outfile("OrdenadasQuicksort.txt");
        ofstream outfile2("TiemposQuicksort.csv", ios::app);  // Modo append para agregar tiempos al CSV

        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo para leer." << endl;
            return 1;
        }

        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo para escribir." << endl;
            return 1;
        }

        std::chrono::duration<double, milli> duracion;  // Variable para medir el tiempo de ejecución
        string line;

        // Leer cada línea del archivo
        while (getline(infile, line)) {
            vector<int> lista;
            istringstream ss(line);
            int number;

            // Convertir la línea en un vector de enteros
            while (ss >> number) {
                lista.push_back(number);
            }

            // Medir el tiempo de ejecución de QuickSort
            auto start = high_resolution_clock::now();
            quickSort(lista, 0 , lista.size() - 1);
            auto end = high_resolution_clock::now();
            
            duracion = end - start;  // Calcular la duración del ordenamiento

            // Escribir la lista ordenada en el archivo de salida
            for (const auto& elemento : lista) {
                outfile << elemento << " ";
            }
        }

        // Mostrar el tiempo transcurrido para la ordenación de la lista actual
        cout <<"Tiempo transcurrido: " << duracion.count() << " ms" << endl;
        
        // Escribir el tiempo de ejecución en el archivo CSV
        outfile2 << duracion.count() << endl;
        
        promedio += duracion.count();  // Acumular los tiempos para calcular el promedio

        // Cerrar archivos
        infile.close();
        outfile.close();
        outfile2.close();
    }

    // Calcular y mostrar el tiempo promedio de ejecución
    cout << "Tiempo promedio: " << promedio / 10 << " ms" << endl;

    return 0;
}
