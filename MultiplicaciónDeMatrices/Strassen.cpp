#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <math.h>

using namespace std;
using matrix = vector<vector<int>>;  // Definimos "matrix" como un alias para representar una matriz 2D de enteros

// Alias para tipos de tiempo que se usan en las mediciones
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;

// Función para imprimir una matriz en el archivo de salida
void printMatrix(ofstream& outfile, const matrix& matrix, const string& matrixName) {
    outfile << matrixName << ":" << endl;
    for (const auto& row : matrix) {
        for (int num : row) {
            outfile << num << " ";  // Imprime cada número de la fila
        }
        outfile << endl;
    }
    outfile << endl;
}

// Función para calcular la siguiente potencia de 2
int nextPowerOfTwo(int x) {
    return pow(2, ceil(log2(x)));  // Devuelve la siguiente potencia de 2 mayor o igual a x
}

// Función para ajustar una matriz a una dimensión que sea una potencia de 2, añadiendo ceros si es necesario
matrix padMatrix(const matrix& mat) {
    int n = mat.size();   // Número de filas de la matriz
    int m = mat[0].size();  // Número de columnas de la matriz
    int newSize = nextPowerOfTwo(max(n, m));  // Calculamos el nuevo tamaño, que será una potencia de 2
    matrix padded(newSize, vector<int>(newSize, 0));  // Creamos una matriz "acolchada" llena de ceros

    // Copiamos los valores de la matriz original a la nueva matriz acolchada
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            padded[i][j] = mat[i][j];
        }
    }
    return padded;
}

// Elimina el acolchado de una matriz para devolverla a su tamaño original
matrix removePadding(const matrix& mat, int originalRows, int originalCols) {
    matrix result(originalRows, vector<int>(originalCols));  // Creamos la matriz con el tamaño original
    for (int i = 0; i < originalRows; ++i) {
        for (int j = 0; j < originalCols; ++j) {
            result[i][j] = mat[i][j];  // Copiamos solo los valores relevantes (sin el acolchado)
        }
    }
    return result;
}

// Función para sumar dos matrices
matrix sumarMatrices(const matrix& A, const matrix& B) {
    int n = A.size();
    matrix C(n, vector<int>(n));  // Creamos una nueva matriz de la misma dimensión
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];  // Sumamos las correspondientes posiciones de A y B
        }
    }
    return C;
}

// Función para restar dos matrices
matrix restarMatrices(const matrix& A, const matrix& B) {
    int n = A.size();
    matrix C(n, vector<int>(n));  // Creamos una nueva matriz de la misma dimensión
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];  // Restamos las correspondientes posiciones de A y B
        }
    }
    return C;
}

// Divide una matriz en cuatro submatrices
void dividirMatriz(const matrix& A, matrix& A11, matrix& A12, matrix& A21, matrix& A22) {
    int n = A.size();
    int m = n / 2;  // Dividimos el tamaño de la matriz por 2
    // Llenamos cada submatriz con la correspondiente sección de la matriz original
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + m];
            A21[i][j] = A[i + m][j];
            A22[i][j] = A[i + m][j + m];
        }
    }
}

// Combina cuatro submatrices en una única matriz
matrix combinarMatrices(const matrix& A11, const matrix& A12, const matrix& A21, const matrix& A22) {
    int n = A11.size() * 2;  // Calculamos el tamaño de la matriz combinada
    matrix C(n, vector<int>(n));  // Creamos la matriz resultante
    int m = A11.size();
    // Copiamos las submatrices en la matriz resultante en sus respectivas posiciones
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            C[i][j] = A11[i][j];
            C[i][j + m] = A12[i][j];
            C[i + m][j] = A21[i][j];
            C[i + m][j + m] = A22[i][j];
        }
    }
    return C;
}

// Algoritmo de Strassen para la multiplicación de matrices
matrix strassen(const matrix& A, const matrix& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};  // Caso base: si las matrices son 1x1, multiplicamos directamente
    }

    if (n == 2) {
        // Caso base para matrices de tamaño 2x2
        matrix temp(2, vector<int>(2));
        temp[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
        temp[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
        temp[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
        temp[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
        return temp;
    }

    // Acolchamos las matrices para que tengan una dimensión que sea potencia de 2
    matrix A_padded = padMatrix(A);
    matrix B_padded = padMatrix(B);
    int newSize = A_padded.size();

    // Creamos las submatrices de A y B
    matrix A11(newSize / 2, vector<int>(newSize / 2)), A12(newSize / 2, vector<int>(newSize / 2)),
           A21(newSize / 2, vector<int>(newSize / 2)), A22(newSize / 2, vector<int>(newSize / 2)),
           B11(newSize / 2, vector<int>(newSize / 2)), B12(newSize / 2, vector<int>(newSize / 2)),
           B21(newSize / 2, vector<int>(newSize / 2)), B22(newSize / 2, vector<int>(newSize / 2)),
           M1(newSize / 2, vector<int>(newSize / 2)), M2(newSize / 2, vector<int>(newSize / 2)),
           M3(newSize / 2, vector<int>(newSize / 2)), M4(newSize / 2, vector<int>(newSize / 2)),
           M5(newSize / 2, vector<int>(newSize / 2)), M6(newSize / 2, vector<int>(newSize / 2)),
           M7(newSize / 2, vector<int>(newSize / 2));

    // Dividimos A y B en sus correspondientes submatrices
    dividirMatriz(A_padded, A11, A12, A21, A22);
    dividirMatriz(B_padded, B11, B12, B21, B22);

    // Calculamos las multiplicaciones intermedias
    auto temp1 = sumarMatrices(A11, A22);
    auto temp2 = sumarMatrices(B11, B22);
    M1 = strassen(temp1, temp2);

    M2 = strassen(sumarMatrices(A21, A22), B11);
    M3 = strassen(A11, restarMatrices(B12, B22));
    M4 = strassen(A22, restarMatrices(B21, B11));
    M5 = strassen(sumarMatrices(A11, A12), B22);
    M6 = strassen(restarMatrices(A21, A11), sumarMatrices(B11, B12));
    M7 = strassen(restarMatrices(A12, A22), sumarMatrices(B21, B22));

    matrix C11 = sumarMatrices(restarMatrices(sumarMatrices(M1, M4), M5), M7);
    matrix C12 = sumarMatrices(M3, M5);
    matrix C21 = sumarMatrices(M2, M4);
    matrix C22 = sumarMatrices(restarMatrices(sumarMatrices(M1, M3), M2), M6);

    matrix C_padded = combinarMatrices(C11, C12, C21, C22);

    return removePadding(C_padded, A.size(), B[0].size());
}


int main() {
    for(int h=0; h<2; h++){
        ifstream infile("Input.txt"); 
        ofstream outfile2("RStrassen.csv", ios::app);
        if (!infile.is_open()) {
            cerr << "No se pudo abrir el archivo 'Input.txt' para leer." << endl;
            return 1;
        }

        ofstream outfile("RStrassen.txt");  
        if (!outfile.is_open()) {
            cerr << "No se pudo abrir el archivo 'RStrassen.txt' para escribir." << endl;
            return 1;
        }

        int rowsA, colsA, rowsB, colsB;

        infile >> rowsA >> colsA;
        matrix matrizA(rowsA, vector<int>(colsA));

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsA; ++j) {
                infile >> matrizA[i][j];
            }
        }

        infile >> rowsB >> colsB;
        matrix matrizB(rowsB, vector<int>(colsB));

        for (int i = 0; i < rowsB; ++i) {
            for (int j = 0; j < colsB; ++j) {
                infile >> matrizB[i][j];
            }
        }
        
        auto start = high_resolution_clock::now();
        matrix resultado = strassen(matrizA, matrizB);
        auto end = high_resolution_clock::now();
        
        chrono::duration<double, milli> duracion = end - start;

        cout << "Tiempo de ejecución: " << duracion.count() << " ms" << std::endl;
        outfile2 << duracion.count() << endl;

        printMatrix(outfile, matrizA, "Matriz A");
        printMatrix(outfile, matrizB, "Matriz B");
        printMatrix(outfile, resultado, "Resultado de la multiplicación");

        infile.close();
        outfile.close();
        outfile2.close();

    }
    return 0;
}