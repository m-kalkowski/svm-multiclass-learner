#ifndef SVM_MULTICLASS_H
#define SVM_MULTICLASS_H

#include <dlib/svm_threaded.h>
#include "features-parser.h"

class SvmMulticlass
{
public:
    SvmMulticlass();
    virtual ~SvmMulticlass();

    void train(sample_type &trainSamples, label_type &trainLabels, std::string fileName);
    void predict(sample_type &testSamples, std::vector<double> &predictedLabels, std::string fileName);
};

#endif // SVM_MULTICLASS_H
