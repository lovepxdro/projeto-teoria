import random
import time
import math
import csv

def merge(arr, l, m, r):
    n1 = m - l + 1
    n2 = r - m

    L = arr[l:m+1]
    R = arr[m+1:r+1]

    i = j = 0
    k = l

    while i < n1 and j < n2:
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1

    while i < n1:
        arr[k] = L[i]
        i += 1
        k += 1

    while j < n2:
        arr[k] = R[j]
        j += 1
        k += 1

def merge_sort(arr, l, r):
    if l < r:
        m = l + (r - l) // 2
        merge_sort(arr, l, m)
        merge_sort(arr, m + 1, r)
        merge(arr, l, m, r)

# Funções para gerar os diferentes casos
def generate_random_array(size):
    return [random.randint(0, size * 10) for _ in range(size)]

def generate_best_case_array(size):
    return [i for i in range(size)]

def generate_worst_case_array(size):
    return [size - i for i in range(size)]

def run_tests():
    input_sizes = [1000, 20000, 100000]
    case_types = ['melhor', 'pior', 'medio']
    num_runs = 30

    print("--- Resultados MergeSort ---")
    print("Tamanho (N) | Caso       | Média (s) | Desvio Padrão (s)")
    print("-------------------------------------------------------")

    results = []

    for size in input_sizes:
        for case_type in case_types:
            execution_times = []

            for _ in range(num_runs):
                # Gera o array conforme o tipo de caso
                if case_type == 'melhor':
                    arr = generate_best_case_array(size)
                elif case_type == 'pior':
                    arr = generate_worst_case_array(size)
                else:  # caso médio
                    arr = generate_random_array(size)

                arr_copy = arr.copy()

                start_time = time.time()
                merge_sort(arr_copy, 0, len(arr_copy) - 1)
                end_time = time.time()

                execution_times.append(end_time - start_time)

            avg_time = sum(execution_times) / num_runs
            std_dev = math.sqrt(sum((x - avg_time) ** 2 for x in execution_times) / (num_runs - 1)) if num_runs > 1 else 0

            print(f"{size:<11} | {case_type:<10} | {avg_time:.6f} | {std_dev:.6f}")

            results.append({
                'N': size,
                'Caso': case_type,
                'mean_time': avg_time,
                'std_dev_time': std_dev
            })

    # Salva os resultados em CSV
    with open('mergesort_results_python.csv', 'w', newline='') as csvfile:
        fieldnames = ['N', 'Caso', 'mean_time', 'std_dev_time']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        for result in results:
            writer.writerow(result)

if __name__ == "__main__":
    random.seed(time.time())
    run_tests()
