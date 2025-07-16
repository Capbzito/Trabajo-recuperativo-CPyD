#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <limits>

const int INF = std::numeric_limits<int>::max();

pthread_mutex_t distances_mutex;

struct MinSearchData {
    int start_index;
    int end_index;
    const std::vector<int>* distancias;
    const std::vector<bool>* visitados;
    int min_dist;  
    int min_index; 
};

struct RelaxData {
    int neighbor_vertex;
    int current_vertex_dist;
    int edge_weight;        
    std::vector<int>* distancias;
};

std::vector<std::vector<int>> parse_matrix_string(const std::string& matrix_str) {
    std::vector<std::vector<int>> grafo;
    std::stringstream ss_rows(matrix_str);
    std::string row_str;

    while (std::getline(ss_rows, row_str, ';')) {
        std::vector<int> row;
        std::stringstream ss_cols(row_str);
        std::string cell_str;
        while (std::getline(ss_cols, cell_str, ',')) {
            row.push_back(std::stoi(cell_str));
        }
        grafo.push_back(row);
    }
    return grafo;
}


void* find_min_task(void* arg) {
    MinSearchData* data = static_cast<MinSearchData*>(arg);

    data->min_dist = INF;
    data->min_index = -1;

    for (int i = data->start_index; i < data->end_index; ++i) {
        if (!(*data->visitados)[i] && (*data->distancias)[i] < data->min_dist) {
            data->min_dist = (*data->distancias)[i];
            data->min_index = i;
        }
    }
    pthread_exit(NULL);
}


void* relax_task(void* arg) {
    RelaxData* data = static_cast<RelaxData*>(arg);

    int new_dist = data->current_vertex_dist + data->edge_weight;

    pthread_mutex_lock(&distances_mutex);
    if (new_dist < (*data->distancias)[data->neighbor_vertex]) {
        (*data->distancias)[data->neighbor_vertex] = new_dist;
    }
    pthread_mutex_unlock(&distances_mutex);

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " \"<matriz>\" \"<inicio>\" \"<salida>\"" << std::endl;
        return 1;
    }

    std::string matriz_str = argv[1];
    int start_vertex = std::stoi(argv[2]);
    std::string output_file = argv[3];

    std::vector<std::vector<int>> grafo = parse_matrix_string(matriz_str);

    int num_vertices = grafo.size();
    if (num_vertices == 0 || start_vertex < 0 || start_vertex >= num_vertices) {
        std::cerr << "Error: Matriz inválida o vértice de inicio fuera de rango." << std::endl;
        return 1;
    }

    pthread_mutex_init(&distances_mutex, NULL);

    std::vector<int> distancias(num_vertices, INF);
    std::vector<bool> visitados(num_vertices, false);
    distancias[start_vertex] = 0;

    int num_min_threads = 4; 
    if (num_min_threads > num_vertices) {
        num_min_threads = num_vertices;
    }


    
    for (int count = 0; count < num_vertices; ++count) {
        std::vector<pthread_t> min_threads(num_min_threads);
        std::vector<MinSearchData> min_data(num_min_threads);

        int chunk_size = num_vertices / num_min_threads;

        for (int i = 0; i < num_min_threads; ++i) {
            min_data[i].start_index = i * chunk_size;
            min_data[i].end_index = (i == num_min_threads - 1) ? num_vertices : (i + 1) * chunk_size;
            min_data[i].distancias = &distancias;
            min_data[i].visitados = &visitados;
            pthread_create(&min_threads[i], NULL, find_min_task, &min_data[i]);
        }

        for (int i = 0; i < num_min_threads; ++i) {
            pthread_join(min_threads[i], NULL);
        }

        int u = -1;
        int min_global_dist = INF;

        for (int i = 0; i < num_min_threads; ++i) {
            if (min_data[i].min_index != -1 && min_data[i].min_dist < min_global_dist) {
                min_global_dist = min_data[i].min_dist;
                u = min_data[i].min_index;
            }
        }

        if (u == -1) {
            break;
        }

        visitados[u] = true;

        std::vector<RelaxData> relax_data_to_process;

        for (int v = 0; v < num_vertices; ++v) {
            if (grafo[u][v] != 0 && grafo[u][v] != INF && !visitados[v]) {
                if (distancias[u] != INF) {
                    RelaxData data;
                    data.neighbor_vertex = v;
                    data.current_vertex_dist = distancias[u];
                    data.edge_weight = grafo[u][v];
                    data.distancias = &distancias;
                    relax_data_to_process.push_back(data);
                }
            }
        }

        std::vector<pthread_t> relax_threads(relax_data_to_process.size()); 

        for (size_t i = 0; i < relax_data_to_process.size(); ++i) {
            pthread_create(&relax_threads[i], NULL, relax_task, &relax_data_to_process[i]);
        }

        for (size_t i = 0; i < relax_threads.size(); ++i) {
            pthread_join(relax_threads[i], NULL);
        }

        for (size_t i = 0; i < relax_threads.size(); ++i) {
            pthread_join(relax_threads[i], NULL);
        }
    }

    pthread_mutex_destroy(&distances_mutex);

    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de salida " << output_file << std::endl;
        return 1;
    }

    outfile << "Vértice\tDistancia desde el origen\n";
    for (int i = 0; i < num_vertices; ++i) {
        if (distancias[i] == INF) {
            outfile << i << "\tINF\n";
        } else {
            outfile << i << "\t" << distancias[i] << "\n";
        }
    }
    outfile.close();

    return 0;
}