#include <iostream>
#include <vector>
#include <random>
#include <fstream>



using namespace std;

// Number of individuals in each generation
#define POPULATION_SIZE 149

// Valid Genes
const string GENES = "kbkbkb";


// Target string to be generated
//const string TARGET = "I love GeeksforGeeks";

// Function to generate random numbers in given range
int random_num(int start, int end)
{

    std::random_device rd;
    std::mt19937 mt(rd());
    srand((unsigned)(time(nullptr)));
    std::uniform_real_distribution<double> dist(start, end);
    return floor(dist(mt));
}

// Create random genes for mutation
char mutated_genes()
{
    int len = GENES.size();
    int r = random_num(0, len-1);
    return GENES[r];
}

// create chromosome or string of genes
string create_gnome()
{
    int len = 100;
    string gnome = "";
    for(int i = 0;i<len;i++)
        gnome += mutated_genes();
    return gnome;
}

// Class representing individual in population
class Individual
{
public:
    string chromosome;
    int fitness;
    Individual(string chromosome);
    Individual mate(Individual parent2);
    int cal_fitness();
};

Individual::Individual(string chromosome)
{
    this->chromosome = chromosome;
    fitness = cal_fitness();
};

// Perform mating and produce new offspring
Individual Individual::mate(Individual par2)
{
    // chromosome for offspring
    string child_chromosome = "";

    int len = chromosome.size();
    for(int i = 0;i<len;i++)
    {
        // random probability
        float p = random_num(0, 100)/100;

        // if prob is less than 0.45, insert gene
        // from parent 1
        if(p < 0.45)
            child_chromosome += chromosome[i];

            // if prob is between 0.45 and 0.90, insert
            // gene from parent 2
        else if(p < 0.90)
            child_chromosome += par2.chromosome[i];

            // otherwise insert random gene(mutate),
            // for maintaining diversity
        else
            child_chromosome += mutated_genes();
    }

    // create new Individual(offspring) using
    // generated chromosome for offspring
    return Individual(child_chromosome);
};


// Calculate fitness score, it is the number of
// characters in string which differ from target
// string.
int Individual::cal_fitness()
{
//    int len = TARGET.size();
//    int fitness = 0;
//    for(int i = 0;i<len;i++)
//    {
//        if(chromosome[i] != TARGET[i])
//            fitness++;
//    }
    int fitness = 0;
//    bool turnOver = false;
    int turnScore = 0;
    int score = 0;
//    int turns = 50;
    int turns = 100;


    for(int i = 0; i < chromosome.size(); i++){
        if(score >= 100){
            turns = i;
            return i;
        }


        int rands = random_num(0, 6);
        if(rands == 1){
            turnScore = 0;
            continue;
        }
        if(chromosome[i] == 'k'){
            turnScore += rands;
            continue;
        } else if(chromosome[i] == 'b') {
            score += turnScore;
            turnScore = 0;
        }
    }
    return turns;
};

// Overloading < operator
bool operator<(const Individual &ind1, const Individual &ind2)
{
    return ind1.fitness < ind2.fitness;
}

string do_operation(){
    int lastFitness = 0;
    int lastFitnessCounter = 0;
    vector<string> names;
//    cout << "Please enter the first player's name: ";


//    srand((unsigned)time(0));

    int generation = 0;

    vector<Individual> population;

//    for(int i = 0;i<POPULATION_SIZE;i++)
//    {
//        string gnome = create_gnome();
//        population.push_back(Individual(gnome));
//    }
    fstream instream;
    instream.open("../best-chromosomes.txt");
    while(true){
        string temp;
        getline(instream, temp);
        if(instream.fail()) break;
        population.push_back(Individual(temp));
    }
    bool found = false;

    while(!found)
    {
        // sort the population in increasing order of fitness score
        sort(population.begin(), population.end());

        // if the individual having lowest fitness score ie.
        // 0 then we know that we have reached to the target
        // and break the loop
        if(population[0].fitness <= 0)
        {
            found = true;
            break;
        }
        if(population[0].fitness == lastFitness){
            lastFitnessCounter++;
        }else{
            lastFitnessCounter = 0;
        }
        if(lastFitnessCounter == 100){
            return population[0].chromosome;
        }

        // Otherwise generate new offsprings for new generation
        vector<Individual> new_generation;

        // Perform Elitism, that mean 10% of fittest population
        // goes to the next generation
        int s = (10*POPULATION_SIZE)/100;
        for(int i = 0;i<s;i++)
            new_generation.push_back(population[i]);

        // From 50% of fittest population, Individuals
        // will mate to produce offspring
        s = (90*POPULATION_SIZE)/100;
        for(int i = 0;i<s;i++)
        {
            int len = population.size();
            int r = random_num(0, 50);
            Individual parent1 = population[r];
            r = random_num(0, 50);
            Individual parent2 = population[r];
            Individual offspring = parent1.mate(parent2);
            new_generation.push_back(offspring);
        }
        population = new_generation;
        cout<< "Generation: " << generation << "\t";
        cout<< "String: "<< population[0].chromosome <<"\t";
        cout<< "Fitness: "<< population[0].fitness << "\n";

        generation++;
        lastFitness = population[0].fitness;
    }
    cout<< "Generation: " << generation << "\t";
    cout<< "String: "<< population[0].chromosome <<"\t";
    cout<< "Fitness: "<< population[0].fitness << "\n";
    return population[0].chromosome;
}

int main() {
    ofstream ost("../best-chromosomes.txt", ofstream::out | ofstream::app);

    for (int i = 0; i < 350; ++i) {
        string chro = do_operation();
        cout << "ITERATION: " << i << endl;
        ost << chro << endl;
    }
    ost.close();

//    while(true){
//        if(names.size() >= 2){break;}
//        string temp;
//        cin >> temp;
//        if(temp == "exit"){
//            break;
//        }
//        names.push_back((temp + ",0"));
//
//        cout << "Please enter the second player's name, exit to exit: ";
//    }
//    int turnCounter = 0;
//    while(true) {
//        for (string s: names) {
//            cout << "Scores: " << s << " ";
//        }
//        string &player = names.at(turnCounter);
//
//        cout << endl;
//        cout << "It is " << player.substr(0, player.find(",")) << "'s turn!" << endl;
//        bool turnOver = false;
//        int turnScore = 0;
//        while(!turnOver){
//            int randoms = rand() % 6 + 1;
//            cout << "Your dice roll is " << randoms;
//            if(randoms != 1){
//                turnScore += randoms;
//                cout << ", and your turn score is " << turnScore << endl;
//
//                cout << "Would you like to (b)ank or (k)eep rolling? ";
//                string temp;
//                cin >> temp;
//                if(temp == "b"){
//                    int oldScore = stoi(player.substr(player.find(",") + 1, player.size()));
//                    player.erase(player.find(",") + 1, player.size());
//                    player.append(to_string(turnScore + oldScore));
//                    turnOver = true;
//                }else if( temp == "k"){
//                    continue;
//                }
//
//            }else{
//                cout << endl;
//                cout << "Your roll was 1! sorry, your turn is over.";
//
//                break;
//            }
//        }
//
//
//
//
//        turnCounter++;
//        if(turnCounter >= names.size()){
//            turnCounter = 0;
//        }
//        bool shouldEnd = false;
//        for (string s: names) {
//            int i = 0;
//            try{
//                i = stoi(s.substr(s.find(",")+1, s.size()));
//            }catch(invalid_argument){
//                cout << "idk what happened";
//            }
//            if(i >= 100){
//                cout << "Congradulations to " << s.substr(0, s.find(",")) << " for winning";
//                shouldEnd = true;
//                break;
//            }
//        }
//        if(shouldEnd){
//            break;
//        }
//    }
    return 0;
}
/*
 * bkkkkkkbkkkkkkkkkbkkkkbkbbkbkkkkbbkkkkkkbbkkkkkkbbbkkbkbkbkbbbbkkbbbbbbkkkkbkkkbkkkkkbkkkkkkkkkkbkbk
kbkkbkkkbkbkkkkkkbbbbkkkbkkkbkkbkbbkkkkkkbkkbbkbkkkbbkbkkkkkbbkkkkkkkkbkbkbbkbbbbkbkkbkkkbkbkkkkbbbb
bkbbkkbkbkbkkkkbkkbkkkkbkkbkkkkkkkbbkbkkkkbkbbkkbbkkkkkkbkbkkkkkkkkkkbkbbbkbbbkbbkkkbkkbkkkkkkbkkbkb
kbkbkkkkkkkkbbbkkbkbkbkkkkbkkbbbkbbbbbbkkbbkkkkkkkbbkkbkbkkkbkbbbkkbkkbkbbkkkbkkbbbbkkbkkkkkkkbbkbkk
bkkkkbkkkbkkkbkkkkkkkbkkkbkkkkkkkkbbbkkkkbkkkkkbkbbbkkbkbbbkkbkbkbbkkbkbkkbbkkbkbkbkkkbbkkkkkkkkbkkk
kkbbkkkkbkkbkkkkkbkbkbkkkbkkbkkkbbkkbbbkkkkbkkkbkbbkbkkkbbbbbkkkkkkkbkkkkkbbbbbkkbkkkkbbkkkkkkkkkbkk
kkkkkbbkbbkbkkbkbbkbkkkbkkbkkkkkbkkkkkbkbbkkkkkkkbkbbkkbkbkbkbkkkkbkkbkkkkkbbkbkbkkkkbkkbkbkkbbkkbbk
kbkkkkkkkbkkkkkkkkbbkkbkkkbbkkkbkkkbbkbbkkkkbkbkkbkbkkkkkbkbkbkbkkkkbbbkkkkbkkbbbkbkkkbkbbbkkbkbkbkb
kkbbkbbkkbkkkkkbbbbkkkbkkbkkkkkkbbkkkkbkkbbkbkkkbkkbkbbkkbkkkbkbkbkbbkkbkbkbkbbbbkkbkbkkkbbkbkbbkkkk
kbkkbkkkkkkkkbbkkkbkbkbkkbbkkkbkkkkkkbkkbkkbkkkkkkbkbkkkkbkkkbbkbkbbkbbbkkbbkkbkkkkkkkkbbkkkkkkbkkbb
bkkkbkkkbbbkkkkbkbkkkkkkkbbkkkkbkkkbkkbkkkbkkbbbbbkkkkkbkkkbbbkkkkkkbkkbbkbbkkkkkkkbbbkbkkbkkbkbbkbk
kkkbkkkkkbkbkkkbkbbkkkbkkkkkkkbkkkkkkkbkkbkkkkbkkkbkkkkbbbkkbkkbbbbkkkkkkbkkkbbbkkkkbbbbkkbkbbkkkkkb
kbbbbkkkkkkkkkkbkkkkbbkkkkbkbbbkbkkkkkkbkkkkkkkbbbkkkkbkbbkbkbkkbbbbkbkbkbbkbkkkkbkkkbkkbkkkkkbkbbkk
bbkkkkbkbkkkkkbbbkkkkkkkkkbkbkbkbkkbkbkkkkkbbkbkkbkkkkkkbkkbkkkbkkbkkbbkbbbbbkkbkkkbkkkbbkbkbkbbbkbb
kkkkkkkkkbbbkkkbkkkbkkbkbkkbkkkkbkkkbkbkkkbkkbbkbkkkkkbkbkkkkbkkkkkkbbkbkbkkbbbkkbbbkbbkkkbkkkkkkkkk
kkbkkbkbkkkkbkbkbkkbkkkkkkkkbkbkkkkkbbbkbkbkkbkkkkkbkkkbbkkkkbkbkkkbkkkkkbkkkbkbkkkkbbkbbbkkkkkbkbkb
kbkbkkbkkbkkbbkkkkkkbkkbbkbbkkkkkkkkkkkkkkkkbbbkkkbbkkbkbkkbkkbbkbbbbbkbkkbkkkkkbbbbkkkbkbbbkbbbbbkk
kkkkkkkkbbkkkkkkbkkbkkkbbkbkkbkkbbkkkbkkbkbkbkkbbkbkkbkkkbkbkbkbkkbkbkbkkbbkkbbkkbbkbkkbkbbbkbkbbkkk
kkkbkkkbbbkkbkkbkkkkkbkkkbbbkkkkbkkkkkkkbkbkkkbkkkkkkbbkbkkkbkbkbbkbbbkbkbkkbbbkkkkkkkkkkbkbkbkkbkkk
kbkkkkkkbkkbbkkkbkkkbbkbkbkkkkbkkkkkkbkbbkkbkkbbkbkkbkkkbbkkbbbkbkkkbbkkkkkbkkkkbkbbbbkkkkbkbbkbbkbk
bkbkkkkbkbbkkkkkkkkkkkkkkkkkkkkkbbkbkkbbkbkkbkkbkkkkkbkkbbkbkkbkbbbkkkkkbkbkbkkbbkbbkkbbbkkkkkbkbbkk
bkbbkbkkkkkbbbkkkkkbbbkkkkkkkkkkkbkkbbkkkkbkbbbkbkkkbbkkbbbkbkbkkkbkkkkkbkkbbkkkbkbbkbkbkbkbkbkbkkkb
kkkkkkkkkbbkbbbkkbbkkbkkbkbkkbkbkkbkkkkbbkbbkkkkbbbkkkkkbkbbbbkbkkbkkkkkbkbkbkbkkbbbbkbkkkkkkkkbkbkk
bkkbkkkkkkkbkkkkkkbbkbkkkkkkkkkbbkkbkbkkbkkbbkkkkkkbkkbkbbkkkbbkbkkbbkbbbkbkkbkkkbkbkkkbkkkkkbbkbkbk
kkbkkbkbkbkkbkkbbbkkkkkbbkbkkkkbkkkkkkbkkkbkkkbkbkbkbkkkbkbkkbbbkkkbkkkbbbbbkbbbkkbkkbbkbbbbkbkkbbkk
kkkkkbkkkbkbbkbkbbkbkkbkkkkkkkbkkkkkkkkkkbkkkbkkbbkbkbbkkkkbkbkbkkkbkkbkbkkkbkbkbkkkkkkbkbkkbkbkkbkb
bkkbkbkkkkbkbkbkbkkkbkbbkkkbbkkbkkbbkkbkbbbkbkkkbkkkkkkkkkkbbkkkkbbkkkkkkkkkkbkkbkkbkkbkkkbkkkkbkbkk
kbkkkkkbbbkbkkbkkkbbkkkkkkkbkkkkbbkkkkbkkkbkkkkkkbbkkbkbkkbkkbbkbkkbbkbkbkbbkkkbkkbkkkbbbkkkkkkbbbkk
kbkkkbkkkkkkbkkbbkkkkkkkkkbbkkkbkkkkbkkbkbbbkbkkkbkbbkkkbkkkkbkkkbkkkkbkbbkkbkkkbkkkkbkbbkkbbkbkbkbb
kkkkkkkkkkkkkkkkkkkbkbbkkbbkkbbbkbbkkkbkkbkbbkkbkbkbkkbbkkkkbkbkbkkkkbbkbkkkbkkbkkkkkkkkkkkkkkbkkbbk
kkkkkkkkkbbkbkkkkkkkbkbkkbbkbkkkkkbkkkkkkkkkbbkkkkkkbbkkbbkkbkkkbkkkkkkkkbbkkkbkkkkkbbkbkkkkbkbkkkbb
bkkkkkkkkkbkbkkkkbkbkkkkbkkbkkkbbkbkkkbkkkkkkkkbkkbkbkbbkkkkbbbbbkkkkbkkbkbkkkkbkkkkkkkbkkbbbbkkkkbk
bkbkbkkbkkkbbkbkbbkkkkkkbkkkbkbbkkkkbkkkkkkbbkkkkkkkkbkbkkkbbkbbkkkkbbbbkkkkbkkkkbkkbkbbkkkkkkkkkkkk
kkkkkkkkbbkbbbbkkkkkkbkbbkkbkkkkbkkkkkkkkkbkkkkkkbkbkkkkbkbkkkkbkbkbbbbkkbkkbbkkkkbbkbkbbkkkkbkkbkbb
bkkbkbbbbkkkkkbkkbkkkbkbkbkkkkkbkbkkbbkkbkkbkkkkkkbkbkkkbkbkbkkbkbkkbkkbkbbbkkkbkkkbbkkbkkbkkbbbbbkb
kbkkkbkkkkbkkkbkkbkkbkbkbkbkbkbbbkbkkbkbkkkbkbbkkkkbkkkkkkkkbkbkbkkbkkbkbkkbkkkbbkbbbbbkbbkkkbbbbkbk
kkbkbkkbkkbkkbkkkkkkbkbkkkkbbbbbkkbkbbkkbkkkkkkkkbbbbkbkkkkbkbkkkkbkkkkkbkkbkbbkkkkkkkkkbkkbkbbbbkbk
kkkkkkkbbkbkkbkkkkkkbbkkkkkkbkkkbkkbbkbkbkbkbkkbkkkkkbkkkbbbbbkkbkbkkbkkbbbkkbkkbbkkbkbbkkbbbbbkkkbb
kkbbkkkkbkkbkkkkbkbbbbkkkkbkbbkkkkkkkkkkkkkkbbkbbkkkkkkkkbkbkbkkkbbkkbkkkkbkkbkkkbbkbbbkbkkbkkbkkbkb
kkbbkbkkbkbkbkbkbkkkkkbkbbkkkkkkkkkbbbbkkkkbkkkkkkbbkkkbkkkkbkbbkbbkbkbbkkbkbkkkkbbkbkkbkkbbkkkbbkkb
kkkkkbbbkkkkkbbkkkbbbkkkkkkbkkkkkkkkbbbkbbkkbkkkkbkkkbkkbkkbbkkkkkbbkbbkbkbkkkkbbkkkkbkbkbkkkbkbkkkk
kbkkkbkbkbkkbbkbkkkbkkbkkkbkkkbbbkkbkkkbkkkkbbkkkbbkkkbkbkkkbkbbkkbbkkbkbkbkbbbbkbbkbbkkbkbkkkbbkkkb
bkkbbkbkkbkkbkkkkkkkkkkbkkkkkbkbkbbkkkbkkkkkkkbkkkkbbbbkbkkkbkkkkkkbkkkkbbkkkbkkkkkbbkkkbkkkkkkkkkkk
kkbkkbkkkbkkbbkkkkbbkbkkkkbkkkkbkbkkkkbkbkkbkbbkkkkbkkkbkbbkkkbkkbkbkkbkkbkkkkkbbkkkbbkkkbkkbkkbkkkk
kkkkkbkkkkkkkbkbbbbkkkkbkkbbkkkbkkkkkbkkbkkkbkkkbkbkkbkkkbkbkbkkbbkbbkbkbkbbkkkkkbbbbkkkkkkkkkbbkbkk
kkkkkkbbkkbkbkbkkkkbkkkkbkkkkkbbkkkbbkbkbkkkkkkbbkbkkkbkbkkbkkkbbkbbbkbkbbbbkkkbkkkbkkbkkkkkkbkkkkkk
kkkkbbkkkkbkkkbbkkkbbkkkbkkkbbkbkkbkkkkkkkbbkkbkkkbkbkkbkkkbbkkkkkbkkkbkbbkkkkkbbbbkbkbkkbkkkkbbbbbk
kbbkkkkbkkkbbkkkkkkkkkkbkkkbkkkbkbkkkbbbkbkkkkbbkkbkbkkkkkbkbkkkbbkbbkbkkbkkkbbkkbkkkkkbbkbkkbkbbkkb
kkbkbkkkkkkkbkkkkkbkkkkkkbkkbbbbkbkkkkbkkkbbkbkkkkkkbkkbkkbkkbbbbkkbbbkbbkkkbkkbbkkbbbkkkkkkkbkkbbbk
kkkbkkkbkkkkkkkkbbkkbbkkbkkkkbkbbbkkbkkkkkbkkkkkbkbbkkkkkbbkkkkkkkbkbkbkkbkbkkbkkbbbbkkkkkkkkkkkkkkk
bkkkkkkkkbkkkkbkbbbkkkkbkkbkbbkbkkbkkkbbbbkkkkkbkbkkkkkkkbkkkkbbkkkbkkkkkbkbkkbkkkkbkkkbbkbkbkkkkkbb
kkkkkbkbbbkkbkkkbkkbkbkbbbbkkkbkkkkbbbkkkbkkkkkbkbkbkkbkkbkbkkbkbkkbkkbbbbkkkbkbbkbkbbbbkbkkkkkkbkkk
kkkkbkkbkbbkkkkbkkkbbkbbkkkkbbkbkkkkkbkkkkkkkbbkkbkkkbkkbkkkbkkbkkkkbbkbbkkbkkbkkbbkkkbkkbbkbkkkbkbk
kkkkkbbkkkkkkkkkkkkkkbkbkkkkbkbkkkkkkkkkkbkkbkkbkbbbbkkkbkbbbkbbbkkbbbkbkkkkbbkbkbkkkkbbkkkbbkkbkbkk
kkbbkkbkbkbkkkbkkbkbbkkkbkbkkbkbkkkkbkkbkkkkkbkkkkbkkkkkkkbkbkkkkkkbkkkbkbbkkkkkbkkbkbbkkbkbkbkbkkbk
bkkbbbkkbbbkbkkkkkkkbkkkkkkkbkkkkbbkkkkkbkbkkbkkkbbkkbkkkkbbkkbkkbkbbkbkkkkkkkkbbkbkbbkkkbbkbkkkkbkk
kkbbkkbkbkkbbkbkkbkkbbkkbkkkkbkkkkkbkkkkkkbkkkbkkbkbkkkkbkbkkkkkbbkkbkkkbkbkkkbkbbkbbbbbkbkkbkkkkbbb
bkbkkkkkkbkkkbbbkkkbkbbkkkkbbkkbkbkbkkbkkkkkkbbkkbbbkkkkkbkkkbkbkbkkkkbkbbbkkkkbbbkkkkbkbkbkbkkbkbkk
kkbkkbkkbkbkbkkkbbkbkkbkkkkkkkbkkkbkbkbbkkkkkkbkkbkkbbbbbbkkbbkkkkkkbkkkkbkbbkkbbkkkkbkkkbkkkkkbbkbk
kkkbkkkkkbkkkbkbbkkkbkkkbkkkbkkkkbkkbkkkkbkkbbbbkkkkkkbkkkkkbkkbkbkbbkkbbkbkkbbbbbbbkkkbbkbbkkkbbbkk
kkkbkkkbbkkbkbkbkkkkkbkkkkbbkbkbbkkbbkkkbkbbbbkkkkkkbbkkbkbkbbbkkkkbbbbkkbkbkkkkbbbkbbbkbkkbkkbkkbkk
kkkkkkbkkkbbbbkkbkbkkkkbkbkkbkbkkbkbkkkkkkkkkbkkkkbkbkbbkkkkkbkkkkkkkkkkbkbbkkbbkkkkkkbkkbkkkkbbkbkb
kkbbbkkkkkkkbkkkbbkkbkkkkbkkkkkbbkkkkkkbkbbbkkkbkkbbkbkkkbbbkkkkbbkbkkkkkbbkkkbkkkkkkkkkbkbbbkbbbbbk
kkkkkbkkkbkkkbbkkbbkkkbkbkbkbkkkkkbkkkkbkkkkkkbkbkkkbbkkbkkbkkbkkbbbkbkbkbbkkbkkkkkkkkbbkkbkkkkkkkbb
kbkkkkkkkkbkbkbkkkbkkbkkkbkkkkkbkbkbkbkkkkbkkbbkkkkbkkkkkbkkkkkbkbkkbbkkkbbbkkkkkbkbbkkkbkbkkkbkkbbk
bkkkkbkkbkkkkbkkkbkkkbkkbbkkkkkkkkbbkkbbbkbkkkbkkkkkbkkkkbkkbkbkkkkkbbkkkkbkkkkkkkkbbbbkkkbkbbbbbkkk
kkbkkkbkkbbbkkkkkkkkbkbkkkkkkkbbbbkkkkkkkkkkkkkbkkkbbkkbkkkbbbbkbkbkkbbkkbkkbkkkkkkbbbkbbbkkbkbkkkkb
kkkkbbbkkkkkkkkkkbbkkbkkkkkkbkkkbkkkkkkkbkkkkkbbkkkbkkkbkkbkbbkkkbkkkbkkkkbkbbkkkkkkbbkbbkkkbkbkkkbb
kkkkbkkkkbbkkkbkkkbkkkkbkkkkkkkbkbbkbkbbkkkbkbkbkkkkkbkbbkkkkkbbbbkbkkkkbkkkbkkbkkkkbkbbkkkkkbkkbkbb
kbkkkbkkkbbbkkkbkkkbkkbbkkbkbkkbkkkkbbbbkkkkkkkbkkkkbkkkkkkbkkkbkkbkbkbkkbbbkbbkkbkbkbkkbkbkbbkkbkbb
kbkkbbkkkkkkkkbkkbbbbkkbbkkkkkkkkkkbkkkbkkkkkkkkkkkkkbbbbkkbbkbkbkbkbbkkbbkbbbkbkkbkbkkkbkkbbkkbkbkb
bkbkkkkkbbkkkkkkkbkkbkkbbkkkkbbbkkkbkkkkkbkkkkkkbbkkbkkkbbkbbbkkbkkkkkbkkkbkbbkkbbkkkkkkbkbkkkbkkkkb
bkkkkkkkbbbbkkkkkkkkbkkkbkkkkkkbbkkkkkkbkbkbbbkkbkkbbbkbbkbkkkkkkbbkkkbkkbbbkkbbkkbbkbkbbbbkbkkkkkkb
kbbbkkkbbkkkkkkkkbbkkkkkbkkkkkkkkkkkbkkkbkkbkbkbkbkbkbkkkbkbbbkkkkkkkbkbbbkkkbkkbkkkkbkbkbkbkbkkkbbk
kkkkkkbkkkkkbkkkbkkkkbkbbkbkkkkbkkkkkbkbbbkbbbkkkkkkkbbkbkkbkbkkbbbkbkkkbkkkkkbkkkbkkkbkkkkbkkbkkkbb
kkkkbkkbkbkkbkkkkkkbbbkkkbbkbkkkkbkbkbkbkkkkbbkbbkkbbkkkkbkkkkkkkkbbbkbbbkbbkkkkkkbkkkbbbkkkkkkkkbkk
bkkkbkkkkkkkbkbkbbkkkkbkkkkkkkkbbkkkbkkkkkkkbkkkbkbkkkkbkkbkbkbbbkkkkkkkkkbkkkbkkkbbkbbkbkkbkbbbkbkk
kkkkkkkbkbkkbkkkbbkkkkbkkbkkkbkkbkkkkkbkbbkkbbkkkkkkkkkkkbbkkkbkkkbkkkbbbkbkbkkbbkkbkbkkbkkbbkbkbbkk
kkkkkbbkkkkkkkkkkkkbkbkbbkkbkbbkkbbkkkbkkkbkkkkkkbkbbbkkbkkbkkkbkkbbbbbkkbbkkkkbkkbkbkkkbkbkkbkbbkbk
bkkkkkkbbkkkbbkbkkkkbkkkbkbkkkkkbkkkkkkbkkkbbkkkbkbbbkkkkkbbbbbkkkbbbkbbbbkkkbkbkkbbbbkkbkkkkkbkbbkb
kbkkkkkkbbkbkkkkkbkkbkkbbkbkkkkkkkbkkkkbkkbkkbkkbkkbkbkkbbbkkkbkkkkbkkbbkkkkbkkkbkkkbbkkkbkbkkbbbbkk
bbbkbbkkkkbkkkkbkbkkkbkbbkbkkkbbkbkkkbkkkbbbbkkkkkkkbkbkbkkbbkkkkkkbkbkbbbkkbkkkkkbbkkkbbkbbkbkbbbbb
bbkkkkkbkbkkkkbbkkkbkbkkbkbbbkbkbkbbkkbbkkbkkkkkbbkkbbkkkkbbkkbkkkkkbbkkkbkbkkbkbbkbkkkkbkbbkbkkbkbk
kkkbkkkbkkkkbkkbbkbkkkkbkbbkkbkkkkkkkbkkbbkbkbkkkkkbkbkkkbkkkbbkkbkkkbkbbbbkbkkbkkbbkkbbkbkkbkbkbkbb
kkkkbbkkkbbbkkkkkkkkkkkbkbkkbbkkkkkkbbkbbkkkkkkkkkkkbkbkkbkbbkbkkbkbkbkkkkkbkkkkkkkkkkkkbkkkkkkkkbbb
kbkkkbkbkbbkkkkkkbkkkkkkkkkkbkkkbkkkkkkkkbkbkkkkbbkkkkbkbkkkkkkbbbkbkkkkkbbkkbbkbbbbkkkkbkkkkkkkkbbb
bkbkkkbkbbkkkbbkkkbkkkkbkbkbkkkbkkbbkkkbkkbkkbkbkkkkkkkkkbkkkbbbkkkbkkbbkkkkkkkbkkbkkbkkbbkkkkkkkbkk
bbkkkkkkbkbkkkbkkbkbkkbkkkkkbbkkkbkkkbkbbkkkbkbbbkbkkkbkbbkkbkbkbkkkbbkkkkkbkkbkkkkkkkkkbbkkkbkbkbkb
kbkkkkkbkkkkbkkkbbkkbkkkkbkkkkbkkkkbbkkkkbkkbbkkkkbbkbkkkkbkkkbbkbkkbkkkkbkkkbkkbkbbkkkkbbkkkkbkkbkb
kbkkkkkkkkkkkbkkkkbbbbbkbbbkkbkkkkkbkbkbbkkbbkkkkbkkkbkkkbbbkkkkkkbkkkkbkbkkkbkbkkkkbkkkkkbkkbkbkkkb
kkkkkkbkkkkkkkkbkkbbkkkbkkkbbbkkkkbbkkkkkkkbkbkbbbkkbkbbbbkkkkkbkkkbkkbkkbbkbbkkbkbkkkbkkbkbkkkkbbbb
kkkbkkkbkbkkbbkbkkbbkbkkbkkkbbkkkkkbkkkkkkkkkbkkbbkkkkkkbkkkkbkkkbkbkkkbkkkbbbbkkbkbkbkkbkbbkkkkbbkk
bkbkbkkbbkkbkkbkkbkkkkkkkkkbkkkbbkkkkbkkbkbkkkkkkkkkkbbbkbkbbbkbkkbbbbbkkkbkbbkbbkbkkkbkbkkkkbkbkkkk
kbkbkkkkbkbkkbkkkkkkkbkkbkkbbkkkkkkkbkbbbbbkkbkbbbkkkbkkbkkkkbkkkbbkkkbkkbkbbkkkbbbbbbbbbkbbkbbkbbkk
bkkkkbkbbkkkbkkkkbbkkbkbbkkbkkkbkkkbkbkkbkkkbkkkbkbbbkbkbkbkkkkkkbkbkbkkkkkkbkkkbkbkkbkkbbbbbkkkkkkb
kkkkkkbkbbkbkkkkkkbkbkkkbbbkkkkkkkkbkbbbkkbkkbkkbbbkkbkkkkbkkkbkbkkkbbbkkbkbkkkbbbkkkkkkbkkkkbbkbbkk
kkkkkkbkbkkbkbbkbbkkbkkkkbbbkkkkkbbkkkkbkkbbkkkbkkkkbkkbkbkkbkkbbkbkkbbkkkkkbkbbkbkkkkbbkkkkkkbkbkkk
kbkkbkbkkkkbkbkkbbkbkbbkkbkkkbkbkkbkkkkkkbbbkkkbbkkkkkkkkkbkkkbkkkkbbkbbkkbkkkbkbkbkbkbbbbkkkbbkkbkk
bkbbkkkkkkkbbbkkkkkkbkkkkkkkkkbkkkbkkkkkbbbkbbkbkbbkbbkkkkkbkkbkkbkbkkbbkkbbbkbkkbkbkkkbkbbbkkkbbbkb
kkkkkkkbkkbbbkkkbkkkbbkbkkkbkbkbkkkkkkkkkkkbkkbkbkkbbkkbkbkkkbkkbkkkkbkkkkkbkkkbbbkkkkkkkkbbkkbkbkbk
kkbkkkbkbkkkbkbkkkkbbkkbkkbkkkbkkkkkbkkkkbkkkkbbbbkbbbkkkbkkbkbbkbkkkbkkkkkbkbkkkkkkbkkkbkkbbbbbkkbk
kkkbkbkkkkkkkkbkbkkkkbkbkbkkkkkbbkkkkbkbkbbkkkbkbkbkkkkkkkkbbbkkkkkbkbbkbkbbkbbkkkkkkbkkkkkkbkkbkkkk
kbkkkkbbkkkbkkkbkkkkbkkkbbkkkkkbkkkbbbbbkkkbkkbkbbkbbkbkkkkbkbkbkkbbkkbbbkbkbkkkkbbbkkkkkkkkkkkkbbkb
kkbkkkkbkkkbkkkkbkkkkkkkkkkbkkkkkbkkkkkbkkbbkkbkkkkkbkkkkkbkbbbbkbkkbbbkkbkbbbkkkkkkkbkbbkbbkkbkbkkk
kkbbbkbkbkkkkkkkbkkkkkbbkbkkbbkkkbkkkkkbkkkkbkkkbbbkbbbbbbkbkbbbbkkkkbbkbkbkkbkkbbkbkkkbkbkkbkkkbbkk
bkbbkkbkkkkkkbkkbbkbkkkbkkbbkkbkkkbkkkbkkkbkkkkkkkkbkkkbkbkkbkbkkkkkbbbkkbbbkbkkkkkbkkbkbbkkkkkkkbbb
bkkkkbkbbkbkkkbbkkbkkkbbbbkkkkbkkkkkkkbkbbkbkkbkbbkkkkkbbbkbkkkkkbkkkbbkbbkbkkkkbbbkbbkkkkkkkkkkkbkb
kkbbkbkkkkkkkkkbkbkbkkkkkkkkkkbbkbbbkkkkkbbbkkkkkkkkbkkkkbkkkkkkkbbbkbkbkbkkkkkkbkbbkkkkkbkkkkkbkbkk
kkkkbbkkbbkkbkkkkkkkkbkkkbbbbbkkkkkbkbkkkkkkkbkkkbkkkbkkbbkbbkkbbkbkkkkbkkkkkkbkkkbbbbkkbbbbkbbkbkkk
bkkkkkkkkkkbbbkkkbkkkbbkkbkbbkkkbbkkkbkbkbkkbkkbkkkbkbkbkkbkkkbkkkkkbkbbkkkkkbkbbkkkkkkkkkkbbbbkkkbk
kbbkkkbkkbkkkkkkkbkkkkkkkkkkkkkkkkkkbbkbkkkbkbbbkkkkkbkkbkkbkkkbkkbbkkbbkkkkkkkkkkkkkbkbbkbbkkbkbkbk
kkkkkkkkbkkbkbkbbkkkkkkbkbkbkbkkkkkbkbkkkkkkbbkkkkkkbkbbkbkkkkkkkkkkbkkkkbbbbkkkbkbkkbbkkkbkbkbbkkbk
kkkkkkkkkkkbkkkbkkbkbkkbkbbkkbkkkkkkbkkkkkbkbbkkbkkkkbkkkbbkbkbkbkkbkkkbbbkkbbbkkbkkkbkkkbkbkkbkbkbk
kkkbkbkkbkkkkkbbkbkbkkbbbkkkkkkkbkbkbbkkbbkkkbkkkkkkkbkkkkkkbbkkkbbkkbbkbkbkkbkbbbkkkbbbbkkkkkbkkbkk
kkkkkkkkbkkkbkbkbkbkkkbkkkkkkkkkkbkkkkbkbkkkbkbkbkkkkkkbkkkkkkbbbkkbbkkkkkkbbkbbbbkkbkkkbkbbkkkbkbbb
kkkkbkkkkkkbkkkkkkkbkkkbbkkkkkkbbkkkkbkbkbkkkkbbkbkbkkkkkkkkbkkbbbkkbkbbbbbkbkkbkkkbkkbkbkbbkkbbkkbb
kkbkkbbkkkkbkkbkkbkkkbkbkbkkkbbkkkbkkkbkkkkkkbbbkkbkkkkbkbkkkbkkbkkkkkkbkbkkbkkkbbkkkbbkbkkkkkkkbbbk
bkkbbkkbkbbkbbbbbkkkkkkkkkkkkbkkbkkkbkkkbbkbbkkbbkbkkkkkkkkbbbkkbkbbkbkkbkkbkkkkkbkbkkkkkkkbkkkkkkkk
bkkkkkkkkkkkkkkkkkbkkkkbbkbkkkkkkbbkkkbkkkkkkkbkkbkkkbbkkkkbkkkbbkbbkbkkbbbkbkkkbbbkbbbkkbkbbkkbkbkb
bkkkbkkbkkkkbkkkbkkbbkkkkkkbkbkkkbkbkkkbkbbkbkkkbkkkkkkbkbbbkkkbkkkkkkbkkkbbkbkbkbkkkbbkkbkbkkkbkbkk
kkkbkkkkbkbkbkkkkkkbbkkkkbkkbkbkkkkkkkbbbkbkkbkkkbkkkbbbkkkbkkkkkbkkkkkkkbkkkkbkbbbbkbbbkbkbkkkkbkbk
kbkkkkkkbbkkkkkkbkkkkbkkkbkkbbkbbkkkkkbkbkkkbbkkkkkkkkbbkkbbkbkkbkkkkbkkbkbbbkbbkbbbkkkkkkbkkbbkbkkb
bbkkbbkkkkkkkbkkkkkkkbkkkkbbkkkkkkkkkkkbkkkkbbkkkbkkkbkkbkkkbkbkkbkkkkkkkbkbbkkbkbkkkbkkkkkbkkbbbkbk
kkkkkkkkkbkkbkkbkkkbkkkkbkbbkkkkkbkbbkkkkkkkbbkbbkkbkbkbkbkbkkbkkbkbkbbbkbkbkbkkkkkkkkkkkkbkbkbbbkbk
bkbkbkkkkkkkkkkbkkbkkkbkbbkkkbkbkbbkkkbbkkkbbkkkkkkbkbkkkbkkbbkkbkbbkbkkbkkkbkbkbbkbbkkkkbbkkkkbbkkk
bbkkkbkbkbkbkkkkkbkkkkkkkkkbkkkkbkbbbkkkbkkkkbkkkkbkkbbkbbkkkbkkkkbbbbkkkbkbkkkkkbbkkkbkbbbkkkkkbbbb
bkkkbkbkbkkbbbbkbkkkkkkkbbkkkkbbbbkkkkkkbkkkbbbkkkkkkkkkkkbbkkbbbkkbbbbbkkkkbkbkkkbkkkbkbkkbkkkkkbkk
kbkkkbkkbbkbbkbbkkkkkkkkkbbbbkkkkbkkkbkkkbbkkkkbkkkkbkbkbkkkkbbbkkbbbbkkbkkbkkkkkkkbkkkkbbbkkkbbkkbb
bkbbkkkkkkbkkkbkbkkkkkkkbbkkbbkkkbkbkkkkbbkbkkbkkkkbkkbbbbkkkbbkkkbkbbbkkkbbkbbkbbkkkbbbkkkbkkbbbkkk
bbkkbkbkkkbkkkkkkkkkkbkkbkbbbkkbbkbbkkkkkkbkkkkbbkkbkkbkkkkkkbbkbkkkbkbbkkkbkbbkkkbkkkkkkbkbkkbbbkkb
kkkbkkkkkkkbkkbkbbkbbkbkkkkbbkkkkbbkkkkbkkbkkkkbbbkkkkkbbbkbbbkkkkkbkbkkkkbbbkkkkkkkkkkbbbbkkbkkbbkk
bbkbbkkkkkbkkbkkkkbkbkkkkbkkkbkbkkkkbbkkbkkkkbkkkkbbkbkbkkkkbkbkbkbbkkbkbbbkkkkkbkkbkkkkbkkkkkbbkbbk
kbkbbkkbkkkkkbbbkkkkbkkbkkkkkkkkkkbkbkkkbkkkkbkkbkbbkkkkkbbbkbkkbkbkbkkkbkbkkkbbkbbbkbbbkbbkkbkkkbbb
bkkbkkkkkkkkbkkbkkkkbkkbkkbkkkkbbkbkkbkkkkkkkkkbbkkkkbbkkbbkkkbkbbkkbkkkbkbkkkkkbkbbbbkbbbkkbkbkbkbb
kbbkbbkkbkkkkkkkkbkkkbkkkbkkbkkbkkkkkbkkkbkkbbkbkbbbbkkkkkkkkkbkkkkbkkbbbkbkkkkbkbkkbkbkkkbbkkbkbkbk
kkbkkkkbbkkbkbbkkkkkbkbkkkkbbkbkkbkkbkkkbbbbbbkkkkkkkbbkkkbbkbbbkkkkkkbbbkkbbbkkbkkkkbkbbkkkkkkkkkbb
bbkkkkbkkkkkkkkkkkbbkbbkkkbkkkbkkbkkbkkkkkkkkbkbkbbkbkkbbkbkbkkkkkbkkkbkbkkbbkbkbkbbbbbbkkkkkbbbbbbk
kkkkbkkkkkkkkkkbkbkkkkbkkkkkkkbkbkkbbbkkbkkbkkkkbkkbbkbkkkkbkkbkbkkkkkkbkkkkbbbbkkbbbbkbbkbkkkbkbbbb
bbbkbkkkkbkkkkkkkbkkkkbkkbkbkkbkkkbkkkbkkkkbbkbkkkbbkkkbkbbkbkkbkbkkbbbkkbkkbkbkbkkkkkkkkkkkkkbkbbbk
kbkkkkkbkkkbkkbbkbkkbkkkkbbkkkbkbbkkbbkbkbkkkkbkkbbkkkkkkkkbkkkbbkbbbbbbkbbkkbbbbkbkbbkkkkkkbbkkbbkk
bkbbkkkbkkkkkkkbkkkbkkkkkbkbkbkbbkbbkkkkkbbkkkkbbbkkbbbkkkkkkbkkkbbbbkkkkkbkbkkbkkbkkbkkkkkbkbkkkbkk
kkkkkbbkkbkbkkkkkbkbkkkkkbkkkkkkkkbkbkkkkbkkkkkkkkkkbkkkkkbbkkkkbkbkbkkkkbkbbkbkbbbkbbkbkbbbkkbkbkkb
bkkkbkkkbkbkkbbkkkkbkbkkbbkkkkbbkkkbkkbkbkkkkkbkbkbkkkkbkkkkkbbkkbkbbkkkkbkkkkkkkkkbbkbkbbkbkbkbbkkb
kkkkkkkbbkbbbkkkkkkkkbkkkkkkkbkbbkkbkbbkkkkbkkkbbbbkbkkbkkkkkkbbbkbbkkbbbkbbkbkkkkbbbkbkkbkkbbkkbbkb
kbbkkkkbkkbkkbkkbkkbbkbbbbkbkbkkbkkkbbkbbkbkkkkkkkkbbkkbkkkbbkkkbbbkbbbbbbkkkkbkkbkbbkkkkkbkkkbkbbkb
bkkkbkkkkkkkkkkkkbkkbkkbkkbbkkbbkbkkbbkbkkkbkkkbkkkbkbkkkbkkkkkkkkkbkkkkkkbkbkbbkkbkkkkbbkbbkkkkbbkb
kbkkkkbkkbbkkkkbkkkkbkbkkkkbkbkkkkkkkbkkbkbbbkbkkbkkbkkkkkkkkkkbbbkkkkkbbbkkbkbbkkbkbkbkkkbbkkkkkbkk
kkkbkbbbkbkbkkbkkkkkkbkkkkkkkkkbkkkkkbkbbbkkbkkkkkbbbbkkbkbkkbbkkkkkbkbbkkkkbbkbbkkkbkkkbbbbkbbbkbbk
kbkkkkbkbkbkkkkkbkkkkbkbkbkbkkbkbkkbkbbkbkkkbkkkkkbkbbkkkbkkkbkbbkkbkkbbbkbkkbkkkkkkkbkbbbbkkkkkkkkk
 */
