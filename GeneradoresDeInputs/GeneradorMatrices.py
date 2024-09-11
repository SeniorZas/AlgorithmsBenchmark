import random

def generate_matrix(rows, cols):
    return [[random.randint(1, 10) for _ in range(cols)] for _ in range(rows)]

def write_matrices_to_file(file_name, matrixA, matrixB):
    with open(file_name, 'w') as file:
        # Write dimensions of Matrix A
        file.write(f"{len(matrixA)} {len(matrixA[0])}\n")
        # Write Matrix A
        for row in matrixA:
            file.write(' '.join(map(str, row)) + '\n')

        file.write('\n')  # Add a blank line between matrices

        # Write dimensions of Matrix B
        file.write(f"{len(matrixB)} {len(matrixB[0])}\n")
        # Write Matrix B
        for row in matrixB:
            file.write(' '.join(map(str, row)) + '\n')

# Example usage
rowsA, colsA = 1024, 1024
rowsB, colsB = 1024, 1024

matrixA = generate_matrix(rowsA, colsA)
matrixB = generate_matrix(rowsB, colsB)

write_matrices_to_file('Input.txt', matrixA, matrixB)

print(f"Matrices generated and written to 'Input.txt'")