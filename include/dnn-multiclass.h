#ifndef DNN_MULTICLASS_H
#define DNN_MULTICLASS_H

#include "features-parser.h"

class DnnMulticlass
{
public:
    DnnMulticlass();
    virtual ~DnnMulticlass();

    void train(sample_type &trainSamples, label_type &trainLabels, std::string fileName);
    void predict(sample_type &testSamples, std::vector<double> &predictedLabels, std::string fileName);
};

#endif // DNN_MULTICLASS_H
