#include <iostream>
#include <algorithm>
#include <chrono>
#include <limits.h>
#include <cmath>
#include <set>
#include <vector>
#include <utility>

using namespace std;

struct City {
    //(x,y) coordinates of city
    double x;
    double y;

    City(double x, double y) : x(x), y(y) {}

    double distance(City other) {
        double xDist = x - other.x;
        double yDist = y - other.y;
        return sqrt((xDist * xDist) + (yDist * yDist));
    }
};

#define Chromosome vector<int>                       //path representation - permutation of cities (encoded with numbers from 0 to N-1)
#define Population vector<pair<double, Chromosome>>1 //collection of chromosomes with their scores

const int MAX_N = 100;
const int MAX_GENERATIONS = 1000;

const int POPULATION_SIZE = 1000;
const int ELITE_SIZE = 100;
const int PARENTS_SIZE = 500;
const int CHILDREN_SIZE = POPULATION_SIZE - ELITE_SIZE;
double MUTATION_RATE = 0.1;
const int MAX_NO_IMPROVEMENT_CNT = 250;

int N; //number of cities == chromosome size
vector<City> cities;
Population population;
Population children;
double distances[MAX_N][MAX_N];

double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void printChromosome(Chromosome c) {
    for(int i = 0; i < N; ++i) {
        printf("%d ", c[i]);
    }
    printf("\n");
}

// Calculates path distance (score of chromosome)
double fitness(const Chromosome& chromosome) {
    double score = 0;
    for(int i = 0; i < N - 1; ++i) {
        score += distances[chromosome[i]][chromosome[i+1]];
    }
    return score;
}

void generateInitialPopulation() {
    population.reserve(POPULATION_SIZE);
    children.resize(CHILDREN_SIZE);

    Chromosome chromosome;
    chromosome.reserve(N);
    for(int i = 0; i < N; ++i) {
        chromosome.push_back(i);
    }

    if(N < 7) {
        //generate all permutations (for N < 7 the number of permutations is less than 1000)
        do {
            population.push_back(make_pair(fitness(chromosome), chromosome));
        }
        while(next_permutation(chromosome.begin(), chromosome.end()));
    }
    else {
        //generate POPULATION_SIZE permutations from the given one
        for(int i = 0; i < POPULATION_SIZE; ++i) {
            random_shuffle(chromosome.begin(), chromosome.end());
            population.push_back(make_pair(fitness(chromosome),chromosome));
        }
    }
    sort(population.begin(), population.end());
}

void initTSP() {
    srand(time(0));

    //generate cities with coordinates
    cities.reserve(MAX_N);
    for(int i = 0; i < N; ++i) {
        cities.push_back(City(fRand(-100, 100), fRand(-100, 100)));
    }

    //calculate distances between cities
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < i; ++j) {
            distances[i][j] = distances[j][i] = cities[i].distance(cities[j]);
        }
    }

    generateInitialPopulation();
}

void onePointCrossover(const Chromosome& parent1, const Chromosome& parent2, Chromosome& child) {
    int crossPoint = 2 + rand() % (N - 4); //crossPoint is in [2,...,N-2]
    bool addedGenes[MAX_N] = {false};

    for(int i = 0; i <= crossPoint; ++i) {
        child[i] = parent1[i];
        addedGenes[parent1[i]] = true;
    }
    int parentIdx = 0;
    for(int i = crossPoint + 1; i < N; ++i) {
        while(addedGenes[parent2[parentIdx]]) {
            ++parentIdx;
        }
        addedGenes[parent2[parentIdx]] = true;
        child[i] = parent2[parentIdx];
    }
}

void twoPointsCrossover(const Chromosome& parent1, const Chromosome& parent2, Chromosome& child) {
    int idx1 = rand() % N;
    int idx2 = rand() % N;
    int startPoint = min(idx1, idx2);
    int endPoint = max(idx1, idx2);

    bool addedGenes[MAX_N] = {false};

    for(int i = startPoint; i <= endPoint; ++i) {
        child[i] = parent1[i];
        addedGenes[parent1[i]] = true;
    }
    int parentIdx = 0;
    for(int i = 0; i < N; ++i) {
        if(i >= startPoint && i <= endPoint) continue;
        while(addedGenes[parent2[parentIdx]]) {
            ++parentIdx;
        }
        child[i] = parent2[parentIdx];
        addedGenes[parent2[parentIdx]] = true;
    }
}

void cycleCrossover(const Chromosome& parent1, const Chromosome& parent2, Chromosome& child) {
    bool addedGenes[MAX_N] = {false};

    vector<int> indexed1(MAX_N), indexed2(MAX_N);
    for(int i = 0; i < N; ++i) {
        indexed1[parent1[i]] = i;
        indexed2[parent2[i]] = i;
    }

    bool fromParentOne = true;
    for(int i = 0; i < N; ++i) {
         if(!addedGenes[parent1[i]] && fromParentOne) {
            int nextIdx = indexed1[parent2[i]];
            addedGenes[parent1[i]] = true;
            child[i] = parent1[i];
            while(nextIdx != i) {
                addedGenes[parent1[nextIdx]] = true;
                child[nextIdx] = parent1[nextIdx];
                nextIdx = indexed1[parent2[nextIdx]];
            }
            fromParentOne = false;
         }
         if(!addedGenes[parent1[i]] && !fromParentOne) {
            int nextIdx = indexed2[parent1[i]];
            addedGenes[parent2[i]] = true;
            child[i] = parent2[i];
            while(nextIdx != i) {
                addedGenes[parent2[nextIdx]] = true;
                child[nextIdx] = parent2[nextIdx];
                nextIdx = indexed2[parent1[nextIdx]];
            }
            fromParentOne = true;
         }
     }
}

void partiallyMappedCrossover(const Chromosome& parent1, const Chromosome& parent2, Chromosome& child) {
    int idx1 = rand() % N;
    int idx2 = rand() % N;
    int startPoint = min(idx1, idx2);
    int endPoint = max(idx1, idx2);

    bool addedGenes[MAX_N] = {false};
    vector<int> indexed2(MAX_N);
    for(int i = 0; i < N; ++i) {
        indexed2[parent2[i]] = i;
    }

    for(int i = startPoint; i <= endPoint; ++i) {
        child[i] = parent1[i];
        addedGenes[parent1[i]] = true;
    }

    for (int i = startPoint; i <= endPoint; ++i)
    {
        int nextIdx = i;
        if(!addedGenes[parent2[i]]) {
            while(nextIdx >= startPoint && nextIdx <= endPoint) {
                nextIdx = indexed2[parent1[nextIdx]];
            }
            child[nextIdx] = parent2[i];
            addedGenes[parent2[i]] = true;
        }
    }

    for(int i = 0; i < N; ++i)
    {
        if(!addedGenes[parent2[i]])
        {
            child[i] = parent2[i];
            addedGenes[parent2[i]] = true;
        }
    }
}

void crossoverByType(int crossoverType, const Chromosome& parent1, const Chromosome& parent2, Chromosome& child) {
    switch(crossoverType) {
        case(0): onePointCrossover(parent1, parent2, child); return;
        case(1): twoPointsCrossover(parent1, parent2, child); return;
        case(2): cycleCrossover(parent1, parent2, child); return;
        case(3): partiallyMappedCrossover(parent1, parent2, child); return;
    }
}

void crossover(const Population& parents) {
    for(int i = 0; i < CHILDREN_SIZE / 2; ++i) {
        int idxParent1 = rand() % PARENTS_SIZE;
        int idxParent2 = rand() % PARENTS_SIZE;
        int crossoverType = rand() % 4;

        Chromosome child(N);
        crossoverByType(crossoverType, parents[idxParent1].second, parents[idxParent2].second, child);
        children[2*i] = make_pair(fitness(child), child);
        crossoverByType(crossoverType, parents[idxParent2].second, parents[idxParent1].second, child);
        children[2*i+1] = make_pair(fitness(child), child);
    }
}

void mutate() {
    for(int i = 0; i < CHILDREN_SIZE; ++i) {
        double randomRation = fRand(0,1);
        if(randomRation <= MUTATION_RATE) {
            int gene1 = rand() % N;
            int gene2 = rand() % N;

            swap(children[i].second[gene1], children[i].second[gene2]);
            children[i].first = fitness(children[i].second);

        }
    }

}

void geneticAlgorithm() {
    //selection: elite + children from best parents (some may mutate)
    population.erase(population.begin() + PARENTS_SIZE, population.end());
    crossover(Population(population));
    mutate();

    population.erase(population.begin() + ELITE_SIZE, population.end());
    population.insert(population.begin() + ELITE_SIZE, children.begin(), children.end());

    sort(population.begin(), population.end());
}

// Traveling Salesman Problem
void TSP() {
    vector<double> bestPathScores;
    bestPathScores.reserve(MAX_GENERATIONS);

    initTSP();
    if(N < 7) {
        printf("The optimal solution is: ");
        printChromosome(population[0].second);
        printf("With score: %f\n", population[0].first);
        return;
    }

    int generationsCnt = 0, noImproveCnt = 0;
    double oldBestScore, newBestScore;
    bestPathScores.push_back(population[0].first);

    while(generationsCnt < MAX_GENERATIONS && noImproveCnt < MAX_NO_IMPROVEMENT_CNT) {
        oldBestScore = population[0].first;

        geneticAlgorithm();
        ++generationsCnt;

        newBestScore = population[0].first;

        if(oldBestScore == newBestScore) {
            ++noImproveCnt;
             MUTATION_RATE += 0.03;
        } else {
            noImproveCnt = 0;
            MUTATION_RATE = 0.1;
        }
        bestPathScores.push_back(newBestScore);
    }

    printf("Generation 0 with best score: %f\n", bestPathScores[0]);
    printf("Generation 10 with best score: %f\n", bestPathScores[10]);
    int generation = bestPathScores.size() / 4;
    printf("Generation %d with best score: %f\n", generation, bestPathScores[generation]);
    generation = bestPathScores.size() / 2;
    printf("Generation %d with best score: %f\n", generation, bestPathScores[generation]);
    generation = 3 * (bestPathScores.size() / 4);
    printf("Generation %d with best score: %f\n", generation, bestPathScores[generation]);
    generation = bestPathScores.size() - 1;
    printf("Generation %d with best score: %f\n", generation, bestPathScores[generation]);
    printf("\nWith best found path: \n");
    printChromosome(population[0].second);
}

int main()
{
    scanf("%d", &N);

    auto startTime = chrono::steady_clock::now();

    TSP();

    auto endTime = chrono::steady_clock::now();
    chrono::duration<double> workTime = endTime - startTime;
    printf("\nWork time: %f seconds\n", workTime.count());


    return 0;
}

