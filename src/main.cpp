#include <iostream>
#include <random>
#include <utility>
#include <chrono>
#include "fonctions.hpp"

#include <algorithm>
#include <iterator>
#include <tuple>

#define ITERATIONS 5000

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
    unsigned int **Div;

    //* Define tabu matrix
    unsigned int **T;
    //* Initialize tabu list length
    const unsigned int l = 10;
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

    //* Initialize diversification matrix
    Div = new unsigned int *[n];
    for (size_t i = 0; i < n; i++)
    {
        Div[i] = new unsigned int[n];
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
    //* Shuffle the initial configuration (random)
    Knuth_Shuffle(permutation_vector, n, generator);

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        //* Add initial configuration to tabu list
        std::copy(permutation_vector, permutation_vector + n, T[tabu_iterator]);

        //* Increment tabu iterator
        tabu_iterator_increment(l, tabu_iterator);

        //* Increment Diversification
        for (size_t i = 0; i < n; i++)
        {
            Div[i][permutation_vector[i]]++;
        }

        //print_tabu_list(T, l, n);

        //* Permutation vector
        unsigned int swaped_vector[n] = {};
        std::copy(permutation_vector, permutation_vector + n, swaped_vector);

        //* Analyse du voisinage
        unsigned int local_min = 1000000;
        unsigned int best_neighbor[n];

#if (1)
        //* If diversification
        if (i % (n * n) == 0)
        {
            unsigned int max = 0;
            unsigned int colonne;
            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                    if (Div[i][j] > max)
                    {
                        max = Div[i][j];
                        colonne = j;
                    }
                }
            }
            std::uniform_int_distribution<unsigned int> rand_int(0, n);
            while (unsigned int value = rand_int(generator) != colonne)
            {
                std::swap(swaped_vector[colonne], swaped_vector[value]);
                break;
            }
        }
#endif

        //* Compute initial sum
        unsigned int initial_sum = compute_sum(n, permutation_vector, D, W);

        for (size_t i = 0; i < (n - 1); i++)
        {
            for (size_t j = i + 1; j < n; j++)
            {
                //+ Swap vector
                std::swap(swaped_vector[i], swaped_vector[j]);

                //+ Calcul de la somme des w[i][j]*d[phi(i)][phi(j)]
                //unsigned int current_neighbor_sum = compute_sum(n, swaped_vector, D, W);

                //+ Calcul de la somme avec les Delta
                int delta = 0;
                for (size_t k = 0; k < n; k++)
                {
                    if (k != i && k != j)
                    {
                        delta += (W[i][k] - W[j][k]) * (D[swaped_vector[i]][swaped_vector[k]] - D[swaped_vector[j]][swaped_vector[k]]);
                    }
                }
                delta *= 2;
                unsigned int current_neighbor_sum = initial_sum + delta;

                if ((current_neighbor_sum < local_min) && !is_tabu(T, swaped_vector, l, n))
                {
                    //+ Update best neighbor not tabu
                    local_min = current_neighbor_sum;
                    std::copy(swaped_vector, swaped_vector + n, best_neighbor);
                    //std::cout << "Le best neighbor vaut : " << best_neighbor[0] << ", " << best_neighbor[1] << ", " << best_neighbor[2] << ", " << best_neighbor[3] << ", " << best_neighbor[4] << ", " << best_neighbor[5] << ", " << best_neighbor[6] << ", " << best_neighbor[7] << ", " << best_neighbor[8] << ", " << best_neighbor[9] << ", " << best_neighbor[10] << ", " << best_neighbor[11] << std::endl;
                    //std::cout << "sum pour ce swap vaut : " << compute_sum(n, best_neighbor, D, W) << std::endl;
                }
                //+ Unswap vector
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
    print_solution(n, best_min, best_configuration);

    std::cout << std::endl;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            std::cout << Div[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    //* Free heap
    for (size_t i = 0; i < n; i++)
    {
        /* code */
    }

    for (size_t i = 0; i < l; i++)
    {
        /* code */
    }
}