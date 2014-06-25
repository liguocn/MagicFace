#include "LogisticRegression.h"
#include "../Math/BasicFunction.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "LinearRegression.h"
#include "../Math/Optimization.h"
#include <math.h>

namespace
{
    class LRGradientDescent : public MagicMath::GradientDescent
    {
    public:
        LRGradientDescent(const std::vector<double>* pDataX, const std::vector<int>* pDataY, std::vector<double>* pCoef);
        ~LRGradientDescent();

    protected:
        virtual void CalInitValue();
        virtual void CalGradient();
        virtual bool IsStop();
        virtual void CalStepLength();
        virtual void UpdateResult();

    private:
        double EnergeValue(const std::vector<double>& coef);

    private:
        const std::vector<double>* mpDataX;
        const std::vector<int>* mpDataY;
        std::vector<double>* mpCoef;
        double mAlpha;
        int mDataDim;
        int mDataCount;
        //cache values
        std::vector<double> mGradVec;
        std::vector<double> mNewCoef;
    };

    LRGradientDescent::LRGradientDescent(const std::vector<double>* pDataX, const std::vector<int>* pDataY, std::vector<double>* pCoef) :
        MagicMath::GradientDescent(1000000),
        mpDataX(pDataX),
        mpDataY(pDataY),
        mpCoef(pCoef),
        mAlpha(1.0),
        mDataDim(pDataX->size() / pDataY->size()),
        mDataCount(pDataY->size()),
        mGradVec(mDataDim + 1, 0),
        mNewCoef(mDataDim + 1, 0)
    {
    }

    LRGradientDescent::~LRGradientDescent()
    {
    }

    void LRGradientDescent::CalInitValue()
    {
        std::vector<double> dataY_d(mpDataY->size());
        for (int dataId = 0; dataId < dataY_d.size(); dataId++)
        {
            dataY_d.at(dataId) = mpDataY->at(dataId);
        }
        MagicML::LinearRegression lr;
        lr.Learn(*mpDataX, dataY_d, dataY_d.size());
        std::vector<double> regMat = lr.GetRegMat();
        mpCoef->clear();
        mpCoef->resize(regMat.size());
        for (int regId = 0; regId < regMat.size(); regId++)
        {
            mpCoef->at(regId) = regMat.at(regId);
        }
    }

    void LRGradientDescent::CalGradient()
    {
        for (std::vector<double>::iterator gradIter = mGradVec.begin(); gradIter != mGradVec.end(); gradIter++)
        {
            *gradIter = 0;
        }
        for (int dataId = 0; dataId < mDataCount; dataId++)
        {
            int dataBaseIndex = dataId * mDataDim;
            double proV = 0;
            for (int did = 0; did < mDataDim; did++)
            {
                proV += mpDataX->at(dataBaseIndex + did) * mpCoef->at(did);
            }
            proV += mpCoef->at(mDataDim);
            proV = MagicMath::BasicFunction::Sigmoid(proV);
            proV = mpDataY->at(dataId) - proV;
            for (int did = 0; did < mDataDim; did++)
            {
                mGradVec.at(did) += mpDataX->at(dataBaseIndex + did) * proV;
            }
            mGradVec.at(mDataDim) += proV;
        }
    }

    bool LRGradientDescent::IsStop()
    {
        double gradLen = 0.0;
        for (std::vector<double>::iterator gradIter = mGradVec.begin(); gradIter != mGradVec.end(); gradIter++)
        {
            gradLen += (*gradIter) * (*gradIter);
        }
        return (gradLen < 1.0e-15);
    }

    void LRGradientDescent::CalStepLength()
    {
        mAlpha *= 2;
        double lastEnerge = EnergeValue(*mpCoef); 
        int coefDim = mDataDim + 1;
        while (true)
        {
            for (int coefId = 0; coefId < coefDim; coefId++)
            {
                mNewCoef.at(coefId) = mpCoef->at(coefId) + mAlpha * mGradVec.at(coefId);
            }
            double newEnerge = EnergeValue(mNewCoef);
            if (newEnerge - lastEnerge < 0)
            {
                mAlpha *= 0.5;
            }
            else
            {
                break;
            }
        }
    }

    void LRGradientDescent::UpdateResult()
    {
        *mpCoef = mNewCoef;
    }

    double LRGradientDescent::EnergeValue(const std::vector<double>& coef)
    {
        double energeV = 0;
        int baseIndex = 0;
        for (int dataId = 0; dataId < mDataCount; dataId++)
        {
            double coefDotX = 0;
            for (int did = 0; did < mDataDim; did++)
            {
                coefDotX += coef.at(did) * mpDataX->at(baseIndex + did);
            }
            coefDotX += coef.at(mDataDim);
            energeV += mpDataY->at(dataId) * coefDotX;
            if (coefDotX > 100)
            {
                energeV -= coefDotX;
            }
            else
            {
                energeV -= log(1.0 + exp(coefDotX));
            }
            baseIndex += mDataDim;
        }
        return energeV;
    }

    class LRNewton : public MagicMath::NewtonMethod
    {
    public:
        LRNewton(const std::vector<double>* pDataX, const std::vector<int>* pDataY, std::vector<double>* pCoef);
        ~LRNewton();

    protected:
        virtual void CalInitValue();
        virtual void CalGradient();
        virtual bool IsStop();
        virtual void CalHessian();
        virtual void UpdateResult();

    private:
        const std::vector<double>* mpDataX;
        const std::vector<int>* mpDataY;
        std::vector<double>* mpCoef;
        int mDataDim;
        int mDataCount;
    };

    LRNewton::LRNewton(const std::vector<double>* pDataX, const std::vector<int>* pDataY, std::vector<double>* pCoef) :
        MagicMath::NewtonMethod(1000, pDataX->size() / pDataY->size() + 1),
        mpDataX(pDataX),
        mpDataY(pDataY),
        mpCoef(pCoef),
        mDataDim(pDataX->size() / pDataY->size()),
        mDataCount(pDataY->size())
    {
    }

    LRNewton::~LRNewton()
    {
    }

    void LRNewton::CalInitValue()
    {
        std::vector<double> dataY_d(mpDataY->size());
        for (int dataId = 0; dataId < dataY_d.size(); dataId++)
        {
            dataY_d.at(dataId) = mpDataY->at(dataId);
        }
        MagicML::LinearRegression lr;
        lr.Learn(*mpDataX, dataY_d, dataY_d.size());
        std::vector<double> regMat = lr.GetRegMat();
        mpCoef->clear();
        mpCoef->resize(regMat.size());
        for (int regId = 0; regId < regMat.size(); regId++)
        {
            mpCoef->at(regId) = regMat.at(regId);
        }
    }

    void LRNewton::CalGradient()
    {
        for (std::vector<double>::iterator gradIter = mGradVec.begin(); gradIter != mGradVec.end(); gradIter++)
        {
            *gradIter = 0;
        }
        for (int dataId = 0; dataId < mDataCount; dataId++)
        {
            int dataBaseIndex = dataId * mDataDim;
            double proV = 0;
            for (int did = 0; did < mDataDim; did++)
            {
                proV += mpDataX->at(dataBaseIndex + did) * mpCoef->at(did);
            }
            proV += mpCoef->at(mDataDim);
            proV = MagicMath::BasicFunction::Sigmoid(proV);
            proV = mpDataY->at(dataId) - proV;
            for (int did = 0; did < mDataDim; did++)
            {
                mGradVec.at(did) += mpDataX->at(dataBaseIndex + did) * proV;
            }
            mGradVec.at(mDataDim) += proV;
        }
    }

    bool LRNewton::IsStop()
    {
        double gradLen = 0.0;
        for (std::vector<double>::iterator gradIter = mGradVec.begin(); gradIter != mGradVec.end(); gradIter++)
        {
            gradLen += (*gradIter) * (*gradIter);
        }
        DebugLog << "grad: " << sqrt(gradLen) << std::endl;
        return (gradLen < 1.0e-15);
    }

    void LRNewton::CalHessian()
    {
        int coefDim = mDataDim + 1;
        mHessMat = std::vector<double>(coefDim * coefDim, 0);
        for (int dataId = 0; dataId < mDataCount; dataId++)
        {
            int dataBaseIndex = dataId * mDataDim;
            double proV = 0;
            for (int did = 0; did < mDataDim; did++)
            {
                proV += mpDataX->at(dataBaseIndex + did) * mpCoef->at(did);
            }
            proV += mpCoef->at(mDataDim);
            proV = MagicMath::BasicFunction::Sigmoid(proV);
            proV = proV * (proV - 1.0);
            for (int rid = 0; rid < mDataDim; rid++)
            {
                int rowBaseIndex = rid * coefDim;
                for (int cid = 0; cid <= rid; cid++)
                {
                    double v = mpDataX->at(dataBaseIndex + rid) * mpDataX->at(dataBaseIndex + cid) * proV;
                    mHessMat.at(rowBaseIndex + cid) += v;
                    mHessMat.at(cid * coefDim + rid) += v;
                }
            }
            int lastRowBaseIndex = mDataDim * coefDim;
            for (int did = 0; did < mDataDim; did++)
            {
                double v = mpDataX->at(dataBaseIndex + did) * proV;
                mHessMat.at(lastRowBaseIndex + did) += v;
                mHessMat.at(coefDim * did + mDataDim) += v;
            }
            mHessMat.at(coefDim * coefDim - 1) += proV;
        }
    }

    void LRNewton::UpdateResult()
    {
        for (int coefId = 0; coefId < mpCoef->size(); coefId++)
        {
            mpCoef->at(coefId) += mStepVec.at(coefId);
        }
    }
}

namespace MagicML
{
    LogisticRegression::LogisticRegression() :
        mCoef()
    {
    }

    LogisticRegression::~LogisticRegression()
    {
    }

    int LogisticRegression::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY)
    {
        if (dataY.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        if (dataX.size() / dataY.size() < 1)
        {
            return MAGIC_INVALID_INPUT;
        }

        /*LRGradientDescent* pLrgd = new LRGradientDescent(&dataX, &dataY, &mCoef);
        pLrgd->Run();
        delete pLrgd;*/
        LRNewton* pLrn = new LRNewton(&dataX, &dataY, &mCoef);
        pLrn->Run();
        delete pLrn;

        return MAGIC_NO_ERROR;
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
    //    //mCoef = std::vector<double>(coefDim, 0);
    //    LearnInitValue(dataX, dataY);
    //    double epsilon = 1.0e-5;
    //    double minStep = 1.0e-8;
    //    double maxStep = 1.0e-5;
    //    int maxIterNum = 100000;
    //    std::vector<double> gradVec(coefDim);
    //    std::vector<double> newCoef(coefDim);
    //    int curIndex = 0;
    //    for (int iterId = 0; iterId < maxIterNum; iterId++)
    //    {
    //        curIndex = rand() % dataCount;
    //        //calculate gradient
    //        int dataBaseIndex = curIndex * dataDim;
    //        double proV = 0;
    //        for (int did = 0; did < dataDim; did++)
    //        {
    //            proV += dataX.at(dataBaseIndex + did) * mCoef.at(did);
    //        }
    //        proV += mCoef.at(dataDim);
    //        proV = MagicMath::BasicFunction::Sigmoid(proV);
    //        proV = dataY.at(curIndex) - proV;
    //        for (int did = 0; did < dataDim; did++)
    //        {
    //            gradVec.at(did) = dataX.at(dataBaseIndex + did) * proV;
    //        }
    //        gradVec.at(dataDim) = proV;
    //        //judge whether to stop
    //        double gradLen = 0.0;
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            gradLen += gradVec.at(coefId) * gradVec.at(coefId);
    //        }
    //        /*if (gradLen < epsilon)
    //        {
    //            DebugLog << "Break iteration: " << iterId << std::endl;
    //            break;
    //        }*/
    //        //update mCoef
    //        double alpha = 1.0;
    //        while (true)
    //        {
    //            for (int coefId = 0; coefId < coefDim; coefId++)
    //            {
    //                newCoef.at(coefId) = mCoef.at(coefId) + alpha * gradVec.at(coefId);
    //            }
    //            double lastEnerge = EnergeValue(dataX, dataY, mCoef); 
    //            double newEnerge = EnergeValue(dataX, dataY, newCoef);
    //            if (newEnerge - lastEnerge < 0)// fabs(lastEnerge) * 0.00001)// && iterId > 2000)
    //            {
    //                alpha *= 0.8;
    //            }
    //            else
    //            {
    //                break;
    //            }
    //        }
    //        //double alpha = maxStep / (1.0 + iterId) + minStep;
    //        for (int coefId = 0; coefId < coefDim; coefId++)
    //        {
    //            mCoef.at(coefId) += alpha * gradVec.at(coefId);
    //        }
    //        DebugLog << "Iteration " << iterId << " : " << gradLen << " " << EnergeValue(dataX, dataY, mCoef) << "  "
    //            << alpha << std::endl;
    //    }       
    //    return MAGIC_NO_ERROR;
    //}

    void LogisticRegression::LearnInitValue(const std::vector<double>& dataX, const std::vector<int>& dataY)
    {
        std::vector<double> dataY_d(dataY.size());
        for (int dataId = 0; dataId < dataY.size(); dataId++)
        {
            dataY_d.at(dataId) = dataY.at(dataId);
        }
        LinearRegression lr;
        lr.Learn(dataX, dataY_d, dataY_d.size());
        std::vector<double> regMat = lr.GetRegMat();
        mCoef.clear();
        mCoef.resize(regMat.size());
        for (int regId = 0; regId < regMat.size(); regId++)
        {
            mCoef.at(regId) = regMat.at(regId);
        }
    }

    double LogisticRegression::EnergeValue(const std::vector<double>& dataX, const std::vector<int>& dataY, 
        const std::vector<double>& coef) const
    {
        int dataCount = dataY.size();
        int dataDim = dataX.size() / dataY.size();
        double energeV = 0;
        int baseIndex = 0;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            double coefDotX = 0;
            //int baseIndex = dataId * dataDim;
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
            baseIndex += dataDim;
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
