#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "mpi.h"

#define MATRIX_ON_VECTOR

void free_matrix(int** matrix, const int size)
{
	for (int i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void matrix_to_array(int** matrix, int* array, const int size)
{
	for (int i = 0; i < size * size; i += size)
	{
		for (int j = 0; j < size; j++)
		{
			array[i + j] = matrix[i / size][j];
		}
	}
}

void array_to_matrix(int** matrix, const int* array, const int size)
{
	for (int i = 0; i < size * size; i += size)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i / size][j] = array[i + j];
		}
	}
}

void init_matrix_rand(int** matrix, const int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i] = (int*)malloc(size * sizeof(int));
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = rand() % 10;
		}
	}
}

void init_matrix(int** matrix, const int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i] = (int*)calloc(size, sizeof(int));
	}
}

void clear_matrix_rows_except_one(int** matrix, const int size, const int exception)
{
	for (int i = 0; i < size; i++)
	{
		if (i != exception)
		{
			matrix[i] = (int*)calloc(size, sizeof(int));
		}
	}
}

void init_vector_rand(int* vector, const int size)
{
	for (int i = 0; i < size; i++)
	{
		vector[i] = rand() % 10;
	}
}

void matrix_on_vector(int** matrix, const int* vector, const int size, int* res)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i] += matrix[i][j] * vector[j];
		}
	}
}

void vector_on_matrix(int** matrix, const int* vector, const int size, int* res)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i] += vector[j] * matrix[j][i];
		}
	}
}

void sum_matrix(int** matrix_base, int** matrix_to_sum, const int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix_base[i][j] += matrix_to_sum[i][j];
		}
	}
}

void sum_vector(int* vector_base, const int* vector_to_sum, const int size)
{
	for (int i = 0; i < size; i++)
	{
		vector_base[i] += vector_to_sum[i];
	}
}

void print_matrix(int** matrix, const int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("Matrix[%i][%i] = %i\t", i, j, matrix[i][j]);
		}
		printf("\n");
	}
}

void print_vector(int* vector, const int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("Vector[%i] = %i\n", i, vector[i]);
	}
}

void copy_matrix(int** to, int** from, const int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}

void copy_vector(int* to, const int* from, const int size)
{
	for (int i = 0; i < size; i++)
	{
		to[i] = from[i];
	}
}


int main(int argc, char* argv[])
{
	srand(time(NULL));
	int task_quantity, rank;
	MPI_Status status;
	int k = 3;

	MPI_Init(&argc, &argv);
	

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &task_quantity);

	printf("Hello MPI From process = %d, total number of processes: %d\n", rank, task_quantity);

	const int size = k * (task_quantity - 1);
	int* result = (int*)calloc(size, sizeof(int));
	int* vector = (int*)calloc(size, sizeof(int));
	int* arr = (int*)malloc(size * size * sizeof(int));
	int** matrix = (int**)malloc(size * sizeof(int*));

	if (rank == 0)
	{
		init_matrix_rand(matrix, size);
		init_vector_rand(vector, size);

		print_matrix(matrix, size);
		print_vector(vector, size);

		double start = MPI_Wtime();


		for (int i = 1; i < size; i += task_quantity - 1)
		{
			for (int j = 1; j < task_quantity; j++)
			{
				int** matrix_copy = (int**)malloc(size * sizeof(int*));
				init_matrix(matrix_copy, size);

				copy_matrix(matrix_copy, matrix, size);
				clear_matrix_rows_except_one(matrix_copy, size, i + j - 2);

				matrix_to_array(matrix_copy, arr, size);

				MPI_Send(arr, size * size, MPI_INT, j, 1, MPI_COMM_WORLD);
				MPI_Send(vector, size, MPI_INT, j, 1, MPI_COMM_WORLD);

				free_matrix(matrix_copy, size);

			}
		}
		for (int i = 1; i < task_quantity; i++)
		{
			MPI_Recv(arr, size, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			sum_vector(result, arr, size);
		}

		printf("Time with parallelization: %f\n", MPI_Wtime() - start);
		printf("res\n");
		print_vector(result, size);
		start = MPI_Wtime();
		matrix_on_vector(matrix, vector, size, arr);
		printf("Time without parallelization: %f\n", MPI_Wtime() - start);
	}
	else
	{
		init_matrix(matrix, size);

		for (int j = 0; j < k; j++)
		{
			MPI_Recv(arr, size * size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(vector, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			array_to_matrix(matrix, arr, size);

#ifdef MATRIX_ON_VECTOR
			matrix_on_vector(matrix, vector, size, result);
#else
			vector_on_matrix(matrix, vector, size, result);
#endif
		}
		printf("mult\n");
		print_vector(result, size);
		MPI_Send(result, size, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	
	free(result);
	free(arr);
	free(vector);
	free_matrix(matrix, size);
	MPI_Finalize();

}

