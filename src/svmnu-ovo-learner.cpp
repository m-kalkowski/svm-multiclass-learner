#include "svmnu-ovo-learner.h"

#include <dlib/svm_threaded.h>


using namespace dlib;

SvmnuOvoLearner::SvmnuOvoLearner()
{

}

SvmnuOvoLearner::~SvmnuOvoLearner()
{

}


void SvmnuOvoLearner::train(sample_type &trainSamples,
                           label_type &trainLabels,
                           std::string modelName)
{
    matrix<double, 0, 0> featuresVector;
    featuresVector.set_size(trainSamples.at(0).size(), 1);

    std::vector<matrix<double, 0, 0>> samples;
    std::vector<double> labels;

    for (size_t i=0; i<trainSamples.size(); ++i) {
        for (size_t j=0; j<trainSamples.at(i).size(); ++j) {
            featuresVector(j) = trainSamples.at(i).at(j);
        }
        samples.push_back(featuresVector);
    }

    std::string prevLabel = trainLabels.at(0);
    double newLabel = 0;
    labels.push_back(newLabel);
    for (size_t i=1; i<trainLabels.size(); ++i) {
        std::string currentLabel = trainLabels.at(i);
        if (currentLabel != prevLabel) {
            newLabel++;
            prevLabel = currentLabel;
        }
        labels.push_back(newLabel);
    }

    vector_normalizer<matrix<double, 0, 0>> normalizer;
    normalizer.train(samples);
    for (unsigned long i=0; i<samples.size(); ++i)
        samples[i] = normalizer(samples[i]);

    randomize_samples(samples, labels);

    typedef one_vs_one_trainer<any_trainer<matrix<double, 0, 0>>> ovo_trainer;
    ovo_trainer trainer;

    typedef radial_basis_kernel<matrix<double, 0, 0>> rbf_kernel;

    svm_nu_trainer<rbf_kernel> rbf_trainer;
    rbf_trainer.set_kernel(rbf_kernel(0.000016));
    rbf_trainer.set_nu(0.21);
    trainer.set_trainer(rbf_trainer);
    std::cout << "cross validation: \n" << cross_validate_multiclass_trainer(trainer, samples, labels, 5) << std::endl;

    normalized_function<one_vs_one_decision_function<ovo_trainer, decision_function<rbf_kernel>>> df;
    df.normalizer = normalizer;
    df.function = trainer.train(samples, labels);

    serialize(modelName + ".dat") << df;
}

void SvmnuOvoLearner::predict(sample_type &testSamples,
                             std::vector<double> &predictedLabels,
                             std::string modelName)
{
    typedef one_vs_one_trainer<any_trainer<matrix<double, 0, 0>>> ovo_trainer;
    typedef radial_basis_kernel<matrix<double, 0, 0>> rbf_kernel;

    normalized_function<one_vs_one_decision_function<ovo_trainer, decision_function<rbf_kernel>>> df;
    deserialize(modelName) >> df;

    matrix<double, 0, 0> featuresVector;
    featuresVector.set_size(testSamples.at(0).size(), 1);

    std::vector<matrix<double, 0, 0>> samples;

    for (size_t i=0; i<testSamples.size(); ++i) {
        for (size_t j=0; j<testSamples.at(i).size(); ++j) {
            featuresVector(j) = testSamples.at(i).at(j);
        }
        samples.push_back(featuresVector);
    }

    for (auto sample : samples)
        predictedLabels.push_back(df(sample));
}

