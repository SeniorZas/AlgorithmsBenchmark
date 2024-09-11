#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm> // Para std::sort

using namespace std;
using namespace std::chrono;

int main() {
    int promedio = 0;  // Variable para acumular el tiempo total de las ejecuciones

    // Repetir el proceso 10 veces para calcular un promedio de tiempos de ejecución
    for (int cuenta = 0; cuenta < 10; cuenta++) {
        ifstream infile("datasetMitadOrdenado100000.txt");  // Archivo de entrada con los datos a ordenar
        ofstream outfile("ordenadasstdsort.txt");           // Archivo de salida para las listas ordenadas

        // Verificar si el archivo de entrada se pudo abrir correctamente
        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo 'datasetMitadOrdenado100000.txt' para leer." << endl;
            return 1;  // Terminar el programa en caso de error
        }

        // Verificar si el archivo de salida se pudo abrir correctamente
        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo 'ordenadasstdsort.txt' para escribir." << endl;
            return 1;  // Terminar el programa en caso de error
        }

        std::chrono::duration<double, milli> duracion;  // Variable para medir la duración en milisegundos
        string line;

        // Empezar el cronómetro
        auto start = high_resolution_clock::now();

        // Leer cada línea del archivo de entrada
        while (getline(infile, line)) {
            vector<int> lista;   // Vector para almacenar los números de la línea
            stringstream ss(line);
            int number;

            // Leer los números en la línea y agregarlos al vector 'lista'
            while (ss >> number) {
                lista.push_back(number);
            }

            // Ordenar la lista utilizando el algoritmo estándar de C++, std::sort
            std::sort(lista.begin(), lista.end());

            // Escribir la lista ordenada en el archivo de salida
            for (const auto &elemento : lista) {
                outfile << elemento << " ";  // Escribe cada número seguido de un espacio
            }
            outfile << endl;  // Nueva línea al final de cada lista ordenada
        }

        // Detener el cronómetro y calcular la duración de la ejecución
        auto stop = high_resolution_clock::now();
        duracion = stop - start;

        // Mostrar el tiempo transcurrido para esta iteración
        cout << "Tiempo transcurrido: " << duracion.count() << " ms" << endl;

        // Acumular el tiempo de esta iteración para calcular el promedio al final
        promedio += duracion.count();

        // Cerrar los archivos de entrada y salida
        infile.close();
        outfile.close();
    }

    // Calcular y mostrar el tiempo promedio de las 10 ejecuciones
    cout << "Tiempo promedio: " << promedio / 10 << " ms" << endl;

    return 0;
}
