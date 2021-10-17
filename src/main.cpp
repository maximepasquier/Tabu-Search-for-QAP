#include <iostream>
#include <random>
#include <utility>
#include <chrono>
#include "fonctions.hpp"

#include <algorithm>
#include <iterator>
#include <tuple>

#define ITERATIONS 1000

unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

int main(int argc, char const *argv[])
{
    std::string path = "./dat.txt";
    //* Get number of facilities
    const unsigned int n = get_n(path);

    //* Define matrices
    unsigned int **D;
    unsigned int **W;

    //* Define tabu matrix
    unsigned int **T;
    //* Initialize tabu list length
    const unsigned int l = 100;
    //* Initialize tabu iterator position
    unsigned int tabu_iterator = 0;

    //* Initialize matrices
    D = new unsigned int *[n];
    W = new unsigned int *[n];
    for (size_t i = 0; i < n; i++)
    {
        D[i] = new unsigned int[n];
        W[i] = new unsigned int[n];
    }

    //* Initialize tabu matrix
    T = new unsigned int *[l];
    for (size_t i = 0; i < l; i++)
    {
        T[i] = new unsigned int[n];
    }

    //* Populate matrices
    read_file(path, D, W);

    //* Best solution
    unsigned int best_min = 1000000;
    unsigned int best_configuration[n];

    //* Initial configuration (random)
    unsigned int permutation_vector[n];
    for (size_t i = 0; i < n; i++)
    {
        permutation_vector[i] = i;
    }
    Knuth_Shuffle(permutation_vector, n, generator);

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        //* Add initial configuration to tabu list
        std::copy(permutation_vector, permutation_vector + n, T[tabu_iterator]);

        //* Increment tabu iterator
        tabu_iterator_increment(l, tabu_iterator);

        //print_tabu_list(T, l, n);

        //* Permutation vector
        unsigned int swaped_vector[n] = {};
        std::copy(permutation_vector, permutation_vector + n, swaped_vector);

        //* Analyse du voisinage
        unsigned int local_min = 1000000;
        unsigned int best_neighbor[n];
        for (size_t i = 0; i < (n - 1); i++)
        {
            for (size_t j = i + 1; j < n; j++)
            {
                // Swap vector
                std::swap(swaped_vector[i], swaped_vector[j]);
                unsigned int current_neighbor_sum = compute_sum(n, swaped_vector, D, W);
                if ((current_neighbor_sum < local_min) && !is_tabu(T, swaped_vector, l, n))
                {
                    //* Update best neighbor not tabu
                    local_min = current_neighbor_sum;
                    std::copy(swaped_vector, swaped_vector + n, best_neighbor);
                    //std::cout << "Le best neighbor vaut : " << best_neighbor[0] << ", " << best_neighbor[1] << ", " << best_neighbor[2] << ", " << best_neighbor[3] << ", " << best_neighbor[4] << ", " << best_neighbor[5] << ", " << best_neighbor[6] << ", " << best_neighbor[7] << ", " << best_neighbor[8] << ", " << best_neighbor[9] << ", " << best_neighbor[10] << ", " << best_neighbor[11] << std::endl;
                    //std::cout << "sum pour ce swap vaut : " << compute_sum(n, best_neighbor, D, W) << std::endl;
                }
                // Unswap vector
                std::swap(swaped_vector[i], swaped_vector[j]);
            }
        }

        //* Update permutation_vector
        std::copy(best_neighbor, best_neighbor + n, permutation_vector);

        //* Update the best solution
        if (local_min < best_min)
        {
            best_min = local_min;
            std::copy(best_neighbor, best_neighbor + n, best_configuration);
        }
    }
    printsolution(n, best_min, best_configuration);
}