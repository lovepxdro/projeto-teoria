#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Gera array aleatório (caso médio)
void generate_random_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (size * 10);
    }
}

// Gera array já ordenado (melhor caso)
void generate_best_case_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

// Gera array em ordem decrescente (pior caso)
void generate_worst_case_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

void copy_array(int src[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

typedef struct {
    int N;
    char* case_type;
    double mean_time;
    double std_dev_time;
} ResultData;

int main() {
    srand(time(NULL));

    int input_sizes[] = {1000, 20000, 100000};
    char* case_types[] = {"melhor", "pior", "medio"};
    int num_sizes = sizeof(input_sizes) / sizeof(input_sizes[0]);
    int num_case_types = sizeof(case_types) / sizeof(case_types[0]);
    int num_runs = 30;

    printf("--- Resultados MergeSort ---\n");
    printf("Tamanho (N) | Caso       | Média (s) | Desvio Padrão (s)\n");
    printf("-------------------------------------------------------\n");

    ResultData results[num_sizes * num_case_types];
    int result_index = 0;

    for (int s_idx = 0; s_idx < num_sizes; ++s_idx) {
        int size = input_sizes[s_idx];

        for (int c_idx = 0; c_idx < num_case_types; ++c_idx) {
            double execution_times[num_runs];
            double sum_times = 0.0;

            int* original_arr = (int*)malloc(size * sizeof(int));
            int* arr_copy = (int*)malloc(size * sizeof(int));

            for (int run = 0; run < num_runs; run++) {
                // Gera o array conforme o tipo de caso
                if (strcmp(case_types[c_idx], "melhor") == 0) {
                    generate_best_case_array(original_arr, size);
                } else if (strcmp(case_types[c_idx], "pior") == 0) {
                    generate_worst_case_array(original_arr, size);
                } else {
                    generate_random_array(original_arr, size);
                }

                copy_array(original_arr, arr_copy, size);

                clock_t start_time = clock();
                mergeSort(arr_copy, 0, size - 1);
                clock_t end_time = clock();

                execution_times[run] = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                sum_times += execution_times[run];
            }

            free(original_arr);
            free(arr_copy);

            double avg_time = sum_times / num_runs;

            double sum_sq_diff = 0.0;
            for (int run = 0; run < num_runs; run++) {
                sum_sq_diff += (execution_times[run] - avg_time) * (execution_times[run] - avg_time);
            }
            double std_dev_time = (num_runs > 1) ? sqrt(sum_sq_diff / (num_runs - 1)) : 0.0;

            printf("%-11d | %-10s | %-9.6f | %-18.6f\n", 
                   size, case_types[c_idx], avg_time, std_dev_time);

            results[result_index].N = size;
            results[result_index].case_type = case_types[c_idx];
            results[result_index].mean_time = avg_time;
            results[result_index].std_dev_time = std_dev_time;
            result_index++;
        }
    }

    FILE *fp = fopen("mergesort_results.csv", "w");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo CSV");
        return 1;
    }
    fprintf(fp, "N,Caso,mean_time,std_dev_time\n");
    for (int i = 0; i < num_sizes * num_case_types; i++) {
        fprintf(fp, "%d,%s,%.6f,%.6f\n", 
                results[i].N, results[i].case_type, results[i].mean_time, results[i].std_dev_time);
    }
    fclose(fp);

    return 0;
}
