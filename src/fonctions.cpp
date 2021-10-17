#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <iterator>
#include "fonctions.hpp"

unsigned int get_n(std::string path)
{
    std::ifstream fichier;
    fichier.open(path);
    unsigned int n;
    fichier >> n;
    return n;
}

void read_file(std::string path, unsigned int **D, unsigned int **W)
{
    std::ifstream fichier;
    fichier.open(path);
    unsigned int n;
    fichier >> n;
    if (fichier.is_open())
    {
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                fichier >> D[i][j];
            }
        }

        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                fichier >> W[i][j];
            }
        }

        fichier.close();
    }
}

unsigned int compute_sum(unsigned int n, unsigned int *swaped_vector, unsigned int **D, unsigned int **W)
{
    unsigned int sum = 0;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            sum += W[i][j] * D[swaped_vector[i]][swaped_vector[j]];
        }
    }
    return sum;
}

void print_tabu_list(unsigned int **T, unsigned int l, unsigned int n)
{
    std::cout << std::endl;
    std::cout << "La tabu list vaut : " << std::endl;
    for (size_t i = 0; i < l; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            std::cout << T[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool is_tabu(unsigned int **T, unsigned int *swaped_vector, unsigned int l, unsigned int n)
{
    for (size_t i = 0; i < l; i++)
    {
        if (std::equal(T[i], T[i] + n, swaped_vector))
        {
            return true;
        }
    }
    return false;
}

void tabu_iterator_increment(unsigned int l, unsigned int &tabu_iterator)
{
    tabu_iterator++;
    tabu_iterator %= l;
}

void printsolution(unsigned int n, unsigned int best_min, unsigned int *best_configuration)
{
    std::cout << std::endl;
    std::cout << "La meilleure configuration trouvée est : (";
    std::cout << best_configuration[0];
    for (size_t i = 1; i < n; i++)
    {
        std::cout << ", " << best_configuration[i];
    }
    std::cout << ") et la somme vaut : " << best_min << std::endl;
}

void Knuth_Shuffle(unsigned int *liste, unsigned int taille, std::default_random_engine generator)
{
    std::uniform_int_distribution<unsigned int> rand_int(0, taille);
    for (int i = taille - 1; i > 0; i--)
    {
        unsigned int index = rand_int(generator) % (i + 1);
        unsigned int tmp = liste[i];
        liste[i] = liste[index];
        liste[index] = tmp;
    }
}