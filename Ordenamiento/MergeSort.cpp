#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Clase abstracta Ordenador
// Define una interfaz para los algoritmos de ordenación, con la función virtual 'ordenar'
template <typename T>
class Ordenador {
public:
    virtual void ordenar(vector<T> &lista) = 0;  // Función virtual pura, que será implementada en las clases derivadas
};

// Implementación de MergeSort, que hereda de la clase abstracta Ordenador
template <typename T>
class OrdenadorMergeSort : public Ordenador<T> {
public:
    // Implementa el método 'ordenar' de la clase base, usando el algoritmo MergeSort
    void ordenar(vector<T> &lista) override {
        if (!lista.empty()) {
            ordenar(lista, 0, lista.size() - 1);  // Llama a la versión privada de ordenar que divide y conquista
        }
    }

private:
    // Método recursivo que divide el vector en dos mitades y luego las ordena
    void ordenar(vector<T> &lista, int posIni, int posFin) {
        if (posIni < posFin) {
            int posCentral = posIni + (posFin - posIni) / 2;  // Encuentra el punto medio
            ordenar(lista, posIni, posCentral);               // Ordena la primera mitad
            ordenar(lista, posCentral + 1, posFin);           // Ordena la segunda mitad
            merge(lista, posIni, posCentral, posFin);         // Combina las dos mitades
        }
    }

    // Función 'merge' que combina dos mitades ya ordenadas en un solo vector ordenado
    void merge(vector<T> &lista, int posIni, int posCentral, int posFin) {
        vector<T> temp(posFin - posIni + 1);  // Vector temporal para almacenar el resultado de la combinación
        int i = posIni, j = posCentral + 1, k = 0;

        // Combina los elementos de ambas mitades en el vector temporal 'temp'
        while (i <= posCentral && j <= posFin) {
            if (lista[i] <= lista[j]) {
                temp[k++] = lista[i++];  // Agrega el elemento de la izquierda si es menor
            } else {
                temp[k++] = lista[j++];  // Agrega el elemento de la derecha si es menor
            }
        }

        // Copia los elementos restantes de la primera mitad (si los hay)
        while (i <= posCentral) {
            temp[k++] = lista[i++];
        }

        // Copia los elementos restantes de la segunda mitad (si los hay)
        while (j <= posFin) {
            temp[k++] = lista[j++];
        }

        // Copia el contenido de 'temp' de vuelta al vector original
        for (i = posIni, k = 0; i <= posFin; ++i, ++k) {
            lista[i] = temp[k];
        }
    }
};

int main() {
    int promedio = 0;  // Variable para almacenar el tiempo promedio de ejecución

    // Repite el proceso de ordenación 10 veces para calcular un tiempo promedio
    for (int cuenta = 0; cuenta < 10; cuenta++) {
        // Abre el archivo de entrada (dataset) y los archivos de salida para los resultados y los tiempos
        ifstream infile("datasets/datasetMitadOrdenado100000.txt");
        ofstream outfile("ordenadasmerge.txt", ios::app);  // Abre en modo 'append' para no sobrescribir
        ofstream outfile2("tiempos.csv", ios::app);        // Archivo para registrar los tiempos

        // Verifica que el archivo de entrada se haya abierto correctamente
        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo 'datasets/datasetDesordenado100000.txt' para leer." << endl;
            return 1;
        }

        // Verifica que el archivo de salida se haya abierto correctamente
        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo 'ordenadasmerge.txt' para escribir." << endl;
            return 1;
        }

        std::chrono::duration<double, milli> duracion;  // Variable para medir la duración en milisegundos
        string line;
        OrdenadorMergeSort<int> ordenador;  // Crea un objeto de la clase OrdenadorMergeSort para ordenar enteros

        // Comienza el cronómetro antes de procesar las líneas del archivo
        auto start = high_resolution_clock::now();

        // Lee el archivo línea por línea
        while (getline(infile, line)) {
            vector<int> lista;  // Vector para almacenar los números de cada línea
            stringstream ss(line);
            int number;

            // Lee los números de la línea y los guarda en el vector 'lista'
            while (ss >> number) {
                lista.push_back(number);
            }

            ordenador.ordenar(lista);  // Ordena la lista usando MergeSort

            // Escribe la lista ordenada en el archivo de salida
            for (const auto &elemento : lista) {
                outfile << elemento << " ";
            }
            outfile << endl;
        }

        // Detiene el cronómetro después de procesar el archivo
        auto stop = high_resolution_clock::now();
        duracion = stop - start;  // Calcula la duración total

        // Muestra el tiempo de ejecución de esta iteración en milisegundos
        cout << "Tiempo transcurrido: " << duracion.count() << " ms" << endl;

        // Acumula el tiempo de ejecución en la variable 'promedio'
        promedio += duracion.count();

        // Escribe el tiempo de ejecución en el archivo 'tiempos.csv'
        outfile2 << duracion.count() << endl;

        // Cierra los archivos
        infile.close();
        outfile.close();
    }

    // Calcula y muestra el tiempo promedio de ejecución
    cout << "Tiempo promedio: " << promedio / 10 << " ms" << endl;

    return 0;
}
