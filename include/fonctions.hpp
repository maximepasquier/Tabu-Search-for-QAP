#include <iostream>

unsigned int get_n(std::string path);
void read_file(std::string path, unsigned int **D, unsigned int **W);
unsigned int compute_sum(unsigned int n, unsigned int *swaped_vector, unsigned int **D, unsigned int **W);
void print_tabu_list(unsigned int **T, unsigned int l, unsigned int n);
void print_solution(unsigned int n, unsigned int best_min, unsigned int *best_configuration);
bool is_tabu(unsigned int **T, unsigned int *swaped_vector, unsigned int l, unsigned int n);
void tabu_iterator_increment(unsigned int l, unsigned int &tabu_iterator);
void Knuth_Shuffle(unsigned int *liste, unsigned int taille, std::default_random_engine generator);