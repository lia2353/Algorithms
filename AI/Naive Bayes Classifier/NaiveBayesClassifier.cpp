#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <math.h>
#include <algorithm>
#include <ctime>

using namespace std;

static const int FEATURES_COUNT = 16;
static const int VALIDATION_SUBSETS_COUNT = 10;

enum FeatureValue {
    YES, NO, UNKNOWN
};
static const int FEATURES_VALUE_COUNT = 3;

enum ClassName {
    REPUBLICAN, DEMOCRAT
};
static const int CLASS_NAME_COUNT = 2;

ClassName parseClassName(const string& answer) {
    if(answer == "republican") {
        return REPUBLICAN;
    } else {  // "democrat"
        return DEMOCRAT;
    }
}

FeatureValue parseFeatureValue (const string& answer) {
    if (answer == "y") {
        return YES;
    } else if (answer == "n") {
        return NO;
    } else {       // "?"
        return UNKNOWN;
    }
}

struct DatasetEntity {
    ClassName className;
    vector<FeatureValue> features;

    DatasetEntity() {
        features.reserve(FEATURES_COUNT);
    }
};

vector<DatasetEntity> dataset;

void prseLine (ifstream& inFile) {
    DatasetEntity entity;
    string answer;

    getline(inFile, answer, ',');
    entity.className = parseClassName(answer);

    int featureNum = 0;
    for (featureNum; featureNum < FEATURES_COUNT - 1; ++featureNum) {
        getline(inFile, answer, ',');
        entity.features.push_back(parseFeatureValue(answer));
    }
    getline(inFile, answer);
    entity.features.push_back(parseFeatureValue(answer));

    dataset.push_back(entity);
}

void readDataset() {
    ifstream inFile("house-votes.data");

    if (inFile.is_open()) {
        while (!inFile.eof()) {
            prseLine(inFile);
        }
        inFile.close();
    }
}


#define PClass map<FeatureValue, float>   // <FeatureValue, P(FeatureValue | Class)>
#define Probabilities vector<pair<PClass, PClass>>  // <P(Republican), P(Democrat)>
#define Republicans first
#define Democrats second

Probabilities probabilities(FEATURES_COUNT);
double PRepublican;
double PDemocrat;
int republicansCount;
int democratsCount;

void init() {
    PRepublican = 0;
    PDemocrat = 0;
    republicansCount = 0;
    democratsCount = 0;

    for (int featureNum = 0; featureNum < FEATURES_COUNT; ++featureNum) {
        probabilities[featureNum].Republicans[YES] = 0;
        probabilities[featureNum].Republicans[NO] = 0;
        probabilities[featureNum].Republicans[UNKNOWN] = 0;

        probabilities[featureNum].Democrats[YES] = 0;
        probabilities[featureNum].Democrats[NO] = 0;
        probabilities[featureNum].Democrats[UNKNOWN] = 0;
    }
}

void calculateProbabilities(vector<DatasetEntity>& data) {
    init();

    for (auto entity : data) {
        if (entity.className == REPUBLICAN) {
            ++republicansCount;
        } else {
            ++democratsCount;
        }

        for (int index = 0; index < FEATURES_COUNT; ++index) {
            if (entity.className == REPUBLICAN) {
                probabilities[index].Republicans[entity.features[index]]++;
            } else {
                probabilities[index].Democrats[entity.features[index]]++;
            }
        }
    }

    // Laplace Smoothing is used to solve the problem of zero probability (with lambda = 1)
    // To overcome data underflow during multiplications, we will use logarithmic probabilities -> log2(a*b) = log2(a) + log2(b)
    const int LAMBDA = 1;

    for (int featureNum = 0; featureNum < FEATURES_COUNT; ++featureNum) {
        probabilities[featureNum].Republicans[YES] = log2( (probabilities[featureNum].Republicans[YES] + LAMBDA) / (double) (republicansCount + LAMBDA * FEATURES_VALUE_COUNT) );
        probabilities[featureNum].Republicans[NO] = log2( (probabilities[featureNum].Republicans[NO] + LAMBDA) / (double) (republicansCount + LAMBDA * FEATURES_VALUE_COUNT) );
        probabilities[featureNum].Republicans[UNKNOWN] = log2( (probabilities[featureNum].Republicans[UNKNOWN] + LAMBDA) / (double) (republicansCount + LAMBDA * FEATURES_VALUE_COUNT) );

        probabilities[featureNum].Democrats[YES] = log2( (probabilities[featureNum].Democrats[YES] + LAMBDA) / (double) (democratsCount + LAMBDA * FEATURES_VALUE_COUNT) );
        probabilities[featureNum].Democrats[NO] = log2( (probabilities[featureNum].Democrats[NO] + LAMBDA) / (double) (democratsCount + LAMBDA * FEATURES_VALUE_COUNT) );
        probabilities[featureNum].Democrats[UNKNOWN] = log2( (probabilities[featureNum].Democrats[UNKNOWN] + LAMBDA) / (double) (democratsCount + LAMBDA * FEATURES_VALUE_COUNT) );
    }

    PRepublican = log2( (republicansCount + LAMBDA) / (double) (republicansCount + democratsCount + LAMBDA * CLASS_NAME_COUNT) );
    PDemocrat = log2( (democratsCount + LAMBDA) / (double) (republicansCount + democratsCount + LAMBDA * CLASS_NAME_COUNT) );
}

ClassName predict(DatasetEntity& entity) {
    double PEntityRepublican = PRepublican;
    double PEntityDemocrat = PDemocrat;

    for (int featureNum = 0; featureNum < FEATURES_COUNT; ++featureNum) {
        PEntityRepublican += probabilities[featureNum].Republicans[entity.features[featureNum]];
        PEntityDemocrat += probabilities[featureNum].Democrats[entity.features[featureNum]];
    }

    if (PEntityRepublican > PEntityDemocrat) {
        return REPUBLICAN;
    } else {
        return DEMOCRAT;
    }
}

int main()
{
    readDataset();

    // 10-fold cross validation -> split to training and testing datasets
    srand(time(0));
    random_shuffle(dataset.begin(), dataset.end());

    double sumAccuracy = 0;
    int subsetSize = dataset.size() / VALIDATION_SUBSETS_COUNT;
    for (int i = 0; i < VALIDATION_SUBSETS_COUNT; ++i) {
        int startIdx = i * subsetSize;
        int endIdx = (i != VALIDATION_SUBSETS_COUNT - 1) ? startIdx + subsetSize : dataset.size();

        vector<DatasetEntity> testingSet(dataset.begin() + startIdx, dataset.begin() + endIdx);
        vector<DatasetEntity> trainingSet(dataset.begin(), dataset.begin() + startIdx);
        trainingSet.insert(trainingSet.end(), dataset.begin() + endIdx, dataset.end());

        calculateProbabilities(trainingSet);

        int correctPredictionsCount = 0;
        for (auto entity : testingSet) {
            ClassName predictionClass = predict(entity);
            if (predictionClass == entity.className) {
                ++correctPredictionsCount;
            }
        }

        double accuracy = correctPredictionsCount / (double) (endIdx - startIdx);
        sumAccuracy += accuracy;
        printf("Test %d accuracy: %f \n", i + 1, accuracy);
    }

    printf("Average  accuracy: %f \n", sumAccuracy / (double) VALIDATION_SUBSETS_COUNT);

    return 0;
}
