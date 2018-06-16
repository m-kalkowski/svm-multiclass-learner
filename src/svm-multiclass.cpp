// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This is an example illustrating the use of the multiclass classification tools
    from the dlib C++ Library.  Specifically, this example will make points from
    three classes and show you how to train a multiclass classifier to recognize
    these three classes.

    The classes are as follows:
        - class 1: points very close to the origin
        - class 2: points on the circle of radius 10 around the origin
        - class 3: points that are on a circle of radius 4 but not around the origin at all
*/

#include <dlib/svm_threaded.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <dlib/rand.h>

using namespace std;
using namespace dlib;

// Our data will be 2-dimensional data. So declare an appropriate type to contain these points.
typedef matrix<double, 0, 0> sample_type;

// ----------------------------------------------------------------------------------------
void parseSessionData(std::vector<std::vector<double> >& trainSamples,
                      std::vector<std::string>& trainLabels,
                      std::vector<std::vector<double> >& testSamples,
                      std::vector<std::string>& testLabels);

// ----------------------------------------------------------------------------------------

/*int main()
{
    try
    {
        // training set of data
        std::vector<std::vector<double> > trainSamples;
        std::vector<std::string> trainLabels;

        // testing set of data
        std::vector<std::vector<double> > testSamples;
        std::vector<std::string> testLabels;

        parseSessionData(trainSamples,
                         trainLabels,
                         testSamples,
                         testLabels);

        // Convert samples to match dlib standards:
        sample_type st_trainSamples;
        st_trainSamples.set_size(trainSamples.at(0).size(), 1);
        std::cout << st_trainSamples.size() << std::endl;
        std::cout << trainLabels.size() << std::endl;

        sample_type st_testSamples;
        st_testSamples.set_size(testSamples.at(0).size(), 1);

        std::vector<sample_type> samples;
        std::vector<double> labels;

        std::vector<sample_type> test_samples;
        std::vector<double> test_labels;

        for (auto i=0; i<trainSamples.size(); ++i) {
            for (auto j=0; j<trainSamples.at(i).size(); ++j) {
                st_trainSamples(j) = trainSamples.at(i).at(j);
            }
            samples.push_back(st_trainSamples);
        }

        for (auto j=0; j<testSamples.size(); ++j) {
            for (auto i=0; i<testSamples.at(j).size(); ++i) {
                st_testSamples(i) = testSamples.at(j).at(i);
            }
            test_samples.push_back(st_testSamples);
        }

        std::string prevLabel = trainLabels.at(0);
        double a = 0;
        labels.push_back(a);
        for (auto j=1; j<trainLabels.size(); ++j) {
            std::string currentLabel = trainLabels.at(j);
            if (currentLabel != prevLabel) {
                a++;
                prevLabel = currentLabel;
            }
            labels.push_back(a);
        }

        prevLabel = testLabels.at(0);
        a = 0;
        for (auto j=0; j<testLabels.size(); ++j) {
            test_labels.push_back(a);
            std::string currentLabel = testLabels.at(j);
            if (currentLabel != prevLabel) {
                a++;
                prevLabel = currentLabel;
            }
        }

        std::cout << labels.size() << std::endl;
        std::cout << samples.size() << std::endl;

        //for (const auto &l : labels)
        //    std::cout << l << std::endl;


        typedef linear_kernel<sample_type> kernel_type;

        svm_multiclass_linear_trainer<kernel_type, double> trainer;
        trainer.set_c(100);
        trainer.set_epsilon(0.000001);


        multiclass_linear_decision_function<kernel_type, double> df = trainer.train(samples, labels);


        for (auto i=0; i<labels.size(); ++i)
            cout << "predicted label: "<< df(samples[i])  << ", true label: "<< labels[i] << endl;

        serialize("df.dat") << df;


        // The output is:
        
        //    predicted label: 2, true label: 2
        //    predicted label: 1, true label: 1
        
    }
    catch (std::exception& e)
    {
        cout << "exception thrown!" << endl;
        cout << e.what() << endl;
    }
}
*/
void parseSessionData(std::vector<std::vector<double> >& trainSamples,
                      std::vector<std::string>& trainLabels,
                      std::vector<std::vector<double> >& testSamples,
                      std::vector<std::string>& testLabels) {

    std::ifstream features("./data/features-diff-channels.txt", std::fstream::in);

    if (!features.is_open())
        return;

    std::string line;
    int lineCount = 0;

    while (getline(features, line)) {

        istringstream lineStream(line);
        std::string value, colon;
        lineStream >>  value >> colon;
        if (lineCount < 1000)
            trainLabels.push_back(value);
        else
            testLabels.push_back(value);

        std::vector<double> frameFeatures;
        while (getline(lineStream, value, ',')) {
            frameFeatures.push_back(std::stod(value));
        }
        if (lineCount < 1000)
            trainSamples.push_back(frameFeatures);
        else
            testSamples.push_back(frameFeatures);
        lineCount++;
    }

/*    for (const auto &l : trainLabels) {
        std::cout << l << ", ";
    }
    std::cout << std::endl;

    for (const auto &t : trainSamples) {
        for (const auto &f : t) {
            std::cout << f << ", ";
        }
        std::cout << std::endl;
    }
*/

    features.close();
}

