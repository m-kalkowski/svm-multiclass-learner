#ifndef I_MACHINE_LEARNER
#define I_MACHINE_LEARNER

#include <string>
#include <vector>

typedef std::vector<std::vector<double>> sample_type;
typedef std::vector<std::string> label_type;


class IMachineLearner {

public:
    virtual ~IMachineLearner() {}

    /**
     * @brief Trains machine based on input space data
     * @param trainSamples Training data
     * @param trainLabels Training labels
     * @param modelName Name of model to be saved without extension
     */
    virtual void train(sample_type &trainSamples,
                       label_type &trainLabels,
                       std::string modelName) = 0;

    /**
     * @brief Predicts the labels based on input space data
     * @param testSamples Testing data
     * @param predictedLabels Predicted Labels
     * @param modelName Name of model to be used to predict without extension
     */
    virtual void predict(sample_type &testSamples,
                         std::vector<double> &predictedLabels,
                         std::string modelName) = 0;
};

#endif // I_MACHINE_LEARNER

