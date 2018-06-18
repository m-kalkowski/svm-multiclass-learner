#include "svm-multiclass.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


using namespace dlib;

SvmMulticlass::SvmMulticlass()
{

}

SvmMulticlass::~SvmMulticlass()
{

}

void SvmMulticlass::train(sample_type &trainSamples, label_type &trainLabels, std::string fileName)
{
    // Convert samples to match dlib standards:
    matrix<double, 0, 0> st_trainSamples;
    st_trainSamples.set_size(trainSamples.at(0).size(), 1);

    std::cout << st_trainSamples.size() << std::endl;
    std::cout << trainLabels.size() << std::endl;


    std::vector<matrix<double, 0, 0>> samples;
    std::vector<double> labels;

    for (auto i=0; i<trainSamples.size(); ++i)
    {
        for (auto j=0; j<trainSamples.at(i).size(); ++j)
        {
            st_trainSamples(j) = trainSamples.at(i).at(j);
        }
        samples.push_back(st_trainSamples);
    }

    std::string prevLabel = trainLabels.at(0);
    double a = 0;
    labels.push_back(a);
    for (auto j=1; j<trainLabels.size(); ++j)
    {
        std::string currentLabel = trainLabels.at(j);
        if (currentLabel != prevLabel)
        {
            a++;
            prevLabel = currentLabel;
        }
        labels.push_back(a);
    }

    std::cout << labels.size() << std::endl;
    std::cout << samples.size() << std::endl;


    typedef linear_kernel<matrix<double, 0, 0>> kernel_type;

    svm_multiclass_linear_trainer<kernel_type, double> trainer;
    trainer.set_c(100);
    trainer.set_epsilon(0.000001);

    multiclass_linear_decision_function<kernel_type, double> df = trainer.train(samples, labels);

    serialize(fileName + ".dat") << df;
}

void SvmMulticlass::predict(sample_type &testSamples, std::vector<double> &predictedLabels, std::string fileName)
{
    typedef linear_kernel<matrix<double, 0, 0>> kernel_type;
    multiclass_linear_decision_function<kernel_type, double> df;

    deserialize(fileName + ".dat") >> df;

    // Convert samples to match dlib standards:
    matrix<double, 0, 0> st_testSamples;
    st_testSamples.set_size(testSamples.at(0).size(), 1);

    std::vector<matrix<double, 0, 0>> samples;

    for (auto i=0; i<testSamples.size(); ++i)
    {
        for (auto j=0; j<testSamples.at(i).size(); ++j)
        {
            st_testSamples(j) = testSamples.at(i).at(j);
        }
        samples.push_back(st_testSamples);
    }

    for (auto sample : samples)
        predictedLabels.push_back(df(sample));
}
