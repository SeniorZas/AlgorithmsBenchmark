#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para obtener el valor máximo en el vector
int getMax(const vector<int>& arr) {
    int mx = arr[0];  // Inicializar el valor máximo con el primer elemento
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > mx)  // Comparar con el valor máximo actual
            mx = arr[i];  // Actualizar si se encuentra un valor mayor
    }
    return mx;  // Devolver el valor máximo encontrado
}

// Función para realizar el Counting Sort basado en el dígito representado por exp (exp=10^i)
void countSort(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);  // Vector auxiliar para almacenar los resultados temporales
    vector<int> count(10, 0);  // Vector de conteo para los dígitos (0 a 9)

    // Contar el número de ocurrencias de cada dígito
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;  // Extraer el dígito de interés con exp
    }

    // Modificar count[i] para que contenga las posiciones finales de los dígitos
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];  // Acumular los conteos
    }

    // Construir el array de salida ordenado basado en el dígito actual
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];  // Ubicar los elementos en la posición correcta
        count[(arr[i] / exp) % 10]--;  // Reducir el conteo
    }

    // Copiar el array ordenado de vuelta a arr[]
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

// Función principal para ordenar el vector usando Radix Sort
void radixSort(vector<int>& arr) {
    int m = getMax(arr);  // Obtener el valor máximo para saber el número de dígitos

    // Aplicar Counting Sort para cada dígito, empezando desde las unidades hasta el dígito más significativo
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countSort(arr, exp);  // Ordenar por el dígito correspondiente
    }
}

// Función para imprimir el vector ordenado (opcional, no usada en este caso)
void print(const vector<int>& arr) {
    for (const auto& elem : arr) {
        cout << elem << " ";  // Imprimir cada elemento separado por espacios
    }
    cout << endl;  // Nueva línea al final de la impresión
}

int main() {
    int totalDuration = 0;  // Variable para acumular la duración total de las iteraciones

    // Realizar el proceso de ordenación 10 veces para calcular un promedio
    for (int cuenta = 0; cuenta < 10; cuenta++) {
        ifstream infile("datasetMitadOrdenado100000.txt");  // Archivo de entrada
        ofstream outfile("OrdenadasRadix.txt");  // Archivo de salida para las listas ordenadas

        string line;

        // Empezar el temporizador general
        auto start = high_resolution_clock::now();

        // Leer cada línea, ordenarla y escribirla en el archivo de salida
        while (getline(infile, line)) {
            vector<int> lista;  // Vector para almacenar los números de la línea
            istringstream ss(line);  // Stream para parsear la línea
            int number;

            // Parsear la línea en una lista de enteros
            while (ss >> number) {
                lista.push_back(number);  // Agregar cada número al vector
            }

            // Medir el tiempo de ejecución de Radix Sort para esta lista
            auto startSort = high_resolution_clock::now();
            radixSort(lista);  // Ordenar la lista con Radix Sort
            auto endSort = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(endSort - startSort);  // Calcular la duración

            // Escribir la lista ordenada en el archivo de salida
            for (const auto& elemento : lista) {
                outfile << elemento << " ";  // Escribir cada número ordenado seguido de un espacio
            }
            outfile << endl;  // Nueva línea al final de cada lista ordenada

            // Mostrar el tiempo transcurrido para la ordenación de la lista actual
            cout << "Tiempo transcurrido para la lista actual: " << duration.count() << " ms" << endl;
        }

        // Detener el temporizador general para esta iteración y calcular la duración total
        auto stop = high_resolution_clock::now();
        auto totalDurationIteration = duration_cast<milliseconds>(stop - start).count();
        totalDuration += totalDurationIteration;  // Acumular la duración total

        infile.close();  // Cerrar archivo de entrada
        outfile.close();  // Cerrar archivo de salida
    }

    // Calcular y mostrar el tiempo promedio de ejecución tras las 10 iteraciones
    cout << "Tiempo promedio: " << totalDuration / 10 << " ms" << endl;

    return 0;
}
