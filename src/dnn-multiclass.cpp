#include "dnn-multiclass.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <dlib/dnn.h>
#include <iostream>
#include <dlib/data_io.h>

using namespace dlib;

DnnMulticlass::DnnMulticlass()
{

}

DnnMulticlass::~DnnMulticlass()
{

}

void DnnMulticlass::train(sample_type &trainSamples, label_type &trainLabels, std::string fileName)
{
    // Convert samples to match dlib standards:
    matrix<double> st_trainSamples;
    st_trainSamples.set_size(trainSamples.at(0).size(), 1);

    std::vector<matrix<double>> samples;
    std::vector<matrix<float>> labels;

    for (auto i=0; i<trainSamples.size(); ++i)
    {
        for (auto j=0; j<trainSamples.at(i).size(); ++j)
        {
            st_trainSamples(j) = trainSamples.at(i).at(j);
        }
        samples.push_back(st_trainSamples);
    }

    std::string prevLabel = trainLabels.at(0);
    float a = 0;
    matrix<float> m(5, 1);
    for (auto i=0; i<5; ++i)
        if (i == a)
            m(i) = 1;
        else
            m(i) = 0;

    labels.push_back(m);
    for (auto j=1; j<trainLabels.size(); ++j)
    {
        std::string currentLabel = trainLabels.at(j);
        if (currentLabel != prevLabel)
        {
            a++;
            prevLabel = currentLabel;
        }
        for (auto i=0; i<5; ++i)
        if (i == a)
            m(i) = 1;
        else
            m(i) = 0;

        labels.push_back(m);
    }

    for (int i=0; i<samples.size(); ++i)
        for (int j=0; j<samples.at(i).size(); ++j)
            samples.at(i)(j) = (samples.at(i)(j) + 3300) / (6600);

    using net_type = loss_mean_squared_multioutput<fc<5, input<matrix<double>>>>;
    
    net_type net;
    layer<1>(net).layer_details().set_bias_learning_rate_multiplier(900);
    sgd defsolver(0,0.9);
    dnn_trainer<net_type> trainer(net, defsolver);
    trainer.set_learning_rate(1e-5);
    trainer.set_min_learning_rate(1e-6);
    trainer.set_mini_batch_size(50);
    trainer.set_max_num_epochs(10000);
    trainer.be_verbose();
    // Finally, this line begins training.  By default, it runs SGD with our specified
    // learning rate until the loss stops decreasing.  Then it reduces the learning rate by
    // a factor of 10 and continues running until the loss stops decreasing again.  It will
    // keep doing this until the learning rate has dropped below the min learning rate
    // defined above or the maximum number of epochs as been executed (defaulted to 10000). 
    trainer.train(samples, labels);

    // At this point our net object should have learned how to classify MNIST images.  But
    // before we try it out let's save it to disk.  Note that, since the trainer has been
    // running images through the network, net will have a bunch of state in it related to
    // the last batch of images it processed (e.g. outputs from each layer).  Since we
    // don't care about saving that kind of stuff to disk we can tell the network to forget
    // about that kind of transient data so that our file will be smaller.  We do this by
    // "cleaning" the network before saving it.
    net.clean();

    serialize(fileName + ".dat") << net;
}

void DnnMulticlass::predict(sample_type &testSamples, std::vector<double> &predictedLabels, std::string fileName)
{
    using net_type = loss_mean_squared_multioutput<fc<5, input<matrix<double>>>>;
    net_type net;
    
    deserialize(fileName + ".dat") >> net;

    // Convert samples to match dlib standards:
    matrix<double> st_testSamples;
    st_testSamples.set_size(testSamples.at(0).size(), 1);

    std::vector<matrix<double>> samples;

    for (auto i=0; i<testSamples.size(); ++i)
    {
        for (auto j=0; j<testSamples.at(i).size(); ++j)
        {
            st_testSamples(j) = testSamples.at(i).at(j);
        }
        samples.push_back(st_testSamples);
    }

    std::vector<matrix<float>> predicted_labels = net(samples);

    for (auto p : predicted_labels)
        std::cout << p << std::endl;
}
