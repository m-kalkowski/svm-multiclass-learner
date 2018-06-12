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

int main()
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

        std::vector<sample_type> samples;
        std::vector<double> labels;

        for (auto j=0; j<trainSamples.size(); ++j) {
            for (auto i=0; i<trainSamples.at(j).size(); ++i) {
                st_trainSamples(i) = trainSamples.at(j).at(i);
            }
            samples.push_back(st_trainSamples);
        }

        std::string prevLabel = trainLabels.at(0);
        double a = 0;
        for (auto j=1; j<trainLabels.size(); ++j) {
            labels.push_back(a);
            std::string currentLabel = trainLabels.at(j);
            if (currentLabel != prevLabel) {
                a++;
                prevLabel = currentLabel;
            }
        }


        for (auto j=0; j<labels.size(); ++j) {
            std::cout << labels.at(j) << ", ";
        }
        std::cout << std::endl << labels.size() << std::endl;


        // TODO: Update those comments
        //
        // The main object in this example program is the one_vs_one_trainer.  It is essentially
        // a container class for regular binary classifier trainer objects.  In particular, it
        // uses the any_trainer object to store any kind of trainer object that implements a
        // .train(samples,labels) function which returns some kind of learned decision function.
        // It uses these binary classifiers to construct a voting multiclass classifier.  If
        // there are N classes then it trains N*(N-1)/2 binary classifiers, one for each pair of
        // labels, which then vote on the label of a sample.
        //
        // In this example program we will work with a one_vs_one_trainer object which stores any
        // kind of trainer that uses our sample_type samples.
        typedef one_vs_one_trainer<any_trainer<sample_type> > ovo_trainer;


        // Finally, make the one_vs_one_trainer.
        ovo_trainer trainer;

        // Next, we will make two different binary classification trainer objects.  One
        // which uses kernel ridge regression and RBF kernels and another which uses a
        // support vector machine and polynomial kernels.  The particular details don't matter.
        // The point of this part of the example is that you can use any kind of trainer object
        // with the one_vs_one_trainer.
        typedef polynomial_kernel<sample_type> poly_kernel;

        // make the binary trainers and set some parameters
        svm_nu_trainer<poly_kernel> poly_trainer;
        poly_trainer.set_kernel(poly_kernel(0.1, 1, 2));


        // Now tell the one_vs_one_trainer that, by default, it should use the rbf_trainer
        // to solve the individual binary classification subproblems.
        trainer.set_trainer(poly_trainer);

        // As an aside, always shuffle the order of the samples before doing cross validation.
        // For a discussion of why this is a good idea see the svm_ex.cpp example.
        randomize_samples(samples, labels);
        //cout << "cross validation: \n" << cross_validate_multiclass_trainer(trainer, samples, labels, 1) << endl;
        // The output is shown below.  It is the confusion matrix which describes the results.  Each row
        // chorresponds to a class of data and each column to a prediction.  Reading from top to bottom,
        // the rows correspond to the class labels if the labels have been listed in sorted order.  So the
        // top row corresponds to class 1, the middle row to class 2, and the bottom row to class 3.  The
        // columns are organized similarly, with the left most column showing how many samples were predicted
        // as members of class 1.
        //
        // So in the results below we can see that, for the class 1 samples, 60 of them were correctly predicted
        // to be members of class 1 and 0 were incorrectly classified.  Similarly, the other two classes of data
        // are perfectly classified.
        /*
            cross validation:
            60  0  0
            0 70  0
            0  0 80
        */

        // Next, if you wanted to obtain the decision rule learned by a one_vs_one_trainer you
        // would store it into a one_vs_one_decision_function.
        one_vs_one_decision_function<ovo_trainer> df = trainer.train(samples, labels);

        cout << "predicted label: "<< df(samples[0])  << ", true label: "<< labels[0] << endl;
        cout << "predicted label: "<< df(samples[500]) << ", true label: "<< labels[500] << endl;

        serialize("model_diff.dat") << df;
        // The output is:
        /*
            predicted label: 2, true label: 2
            predicted label: 1, true label: 1
        */
    }
    catch (std::exception& e)
    {
        cout << "exception thrown!" << endl;
        cout << e.what() << endl;
    }
}

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
        if (lineCount <= 1000)
            trainLabels.push_back(value);
        else
            testLabels.push_back(value);

        std::vector<double> frameFeatures;
        while (getline(lineStream, value, ',')) {
             frameFeatures.push_back(std::stod(value));
        }
        if (lineCount <= 1000)
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

