#include "LogisticRegression.h"
#include "../Math/BasicFunction.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <math.h>

namespace MagicML
{
    LogisticRegression::LogisticRegression() :
        mCoef()
    {
    }

    LogisticRegression::~LogisticRegression()
    {
    }

    //int LogisticRegression::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY)
    //{
    //    int dataCount = dataY.size();
    //    if (dataCount == 0)
    //    {
    //        return MAGIC_EMPTY_INPUT;
    //    }
    //    int dataDim = dataX.size() / dataY.size();
    //    if (dataDim < 1)
    //    {
    //        return MAGIC_INVALID_INPUT;
    //    }
    //    int coefDim = dataDim + 1;
    //    mCoef = std::vector<double>(coefDim, 0);
    //    double epsilon = 1.0e-15;
    //    double alpha = 1.0e-4;
    //    int maxIterNum = 10000;
    //    std::vector<double> gradVec(coefDim);
    //    std::vector<double> newCoef(coefDim);
    //    for (int iterId = 0; iterId < maxIterNum; iterId++)
    //    {
    //        //calculate gradient
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            gradVec.at(coefId) = 0;
    //        }
    //        for (int dataId = 0; dataId < dataCount; dataId++)
    //        {
    //            int dataBaseIndex = dataId * dataDim;
    //            double proV = 0;
    //            for (int did = 0; did < dataDim; did++)
    //            {
    //                proV += dataX.at(dataBaseIndex + did) * mCoef.at(did);
    //            }
    //            proV += mCoef.at(dataDim);
    //            proV = MagicMath::BasicFunction::Sigmoid(proV);
    //            proV = dataY.at(dataId) - proV;
    //            for (int did = 0; did < dataDim; did++)
    //            {
    //                gradVec.at(did) += dataX.at(dataBaseIndex + did) * proV;
    //            }
    //            gradVec.at(dataDim) += proV;
    //        }
    //        //judge whether to stop
    //        double gradLen = 0.0;
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            gradLen += gradVec.at(coefId) * gradVec.at(coefId);
    //        }
    //        gradLen = sqrt(gradLen);
    //        if (gradLen < epsilon)
    //        {
    //            DebugLog << "Break iteration: " << iterId << std::endl;
    //            break;
    //        }
    //        //update mCoef
    //        /*for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            newCoef.at(coefId) += alpha * gradVec.at(coefId);
    //        }
    //        double lastEnerge = EnergeValue(dataX, dataY, mCoef); 
    //        double newEnerge = EnergeValue(dataX, dataY, newCoef);
    //        if (newEnerge - lastEnerge < fabs(lastEnerge) * 0.0001 && iterId > 2000)
    //        {
    //            alpha *= 0.5;
    //        }*/
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            mCoef.at(coefId) += alpha * gradVec.at(coefId);
    //        }
    //        DebugLog << "Iteration " << iterId << " : " << gradLen << " " << EnergeValue(dataX, dataY, mCoef) << "  ";
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            DebugLog << mCoef.at(coefId) << " ";
    //        }
    //        DebugLog << std::endl;
    //    }
    //    return MAGIC_NO_ERROR;
    //}

    int LogisticRegression::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY)
    {
        int dataCount = dataY.size();
        if (dataCount == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        int dataDim = dataX.size() / dataY.size();
        if (dataDim < 1)
        {
            return MAGIC_INVALID_INPUT;
        }
        int coefDim = dataDim + 1;
        mCoef = std::vector<double>(coefDim, 0);
        double epsilon = 1.0e-5;
        double minStep = 1.0e-3;
        double maxStep = 1.0e-4;
        int maxIterNum = 100000;
        std::vector<double> gradVec(coefDim);
        std::vector<double> newCoef(coefDim);
        int curIndex = 0;
        for (int iterId = 0; iterId < maxIterNum; iterId++)
        {
            curIndex = rand() % dataCount;
            //calculate gradient
            int dataBaseIndex = curIndex * dataDim;
            double proV = 0;
            for (int did = 0; did < dataDim; did++)
            {
                proV += dataX.at(dataBaseIndex + did) * mCoef.at(did);
            }
            proV += mCoef.at(dataDim);
            proV = MagicMath::BasicFunction::Sigmoid(proV);
            proV = dataY.at(curIndex) - proV;
            for (int did = 0; did < dataDim; did++)
            {
                gradVec.at(did) = dataX.at(dataBaseIndex + did) * proV;
            }
            gradVec.at(dataDim) = proV;
            //judge whether to stop
            double gradLen = 0.0;
            for (int coefId = 0; coefId < coefDim; coefId++)
            {
                gradLen += gradVec.at(coefId) * gradVec.at(coefId);
            }
            /*if (gradLen < epsilon)
            {
                DebugLog << "Break iteration: " << iterId << std::endl;
                break;
            }*/
            //update mCoef
            double alpha = maxStep / (1.0 + iterId) + minStep;
            for (int coefId = 0; coefId < coefDim; coefId++)
            {
                mCoef.at(coefId) += alpha * gradVec.at(coefId);
            }
            DebugLog << "Iteration " << iterId << " : " << curIndex << " " << gradLen << " " << EnergeValue(dataX, dataY, mCoef) << "  ";
            for (int coefId = 0; coefId < coefDim; coefId++)
            {
                DebugLog << mCoef.at(coefId) << " ";
            }
            DebugLog << std::endl;
        }
        return MAGIC_NO_ERROR;
    }

    double LogisticRegression::EnergeValue(const std::vector<double>& dataX, const std::vector<int>& dataY, 
        const std::vector<double>& coef) const
    {
        int dataCount = dataY.size();
        int dataDim = dataX.size() / dataY.size();
        double energeV = 0;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            double coefDotX = 0;
            int baseIndex = dataId * dataDim;
            for (int did = 0; did < dataDim; did++)
            {
                coefDotX += coef.at(did) * dataX.at(baseIndex + did);
            }
            coefDotX += coef.at(dataDim);
            energeV += dataY.at(dataId) * coefDotX;
            if (coefDotX > 100)
            {
                energeV -= coefDotX;
            }
            else
            {
                energeV -= log(1.0 + exp(coefDotX));
            }
        }
        return energeV;
    }

    double LogisticRegression::Predict(const std::vector<double>& dataX) const
    {
        double res = 0.0;
        int dataDim = dataX.size();
        for (int did = 0; did < dataDim; did++)
        {
            res += dataX.at(did) * mCoef.at(did);
        }
        res += mCoef.at(dataDim);
        return MagicMath::BasicFunction::Sigmoid(res);
    }
}
