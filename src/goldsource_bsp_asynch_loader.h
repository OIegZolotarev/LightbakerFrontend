/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "loader_thread.h"

namespace GoldSource
{
class BSPLevel;
class BSPWorld;

class BSPAsynchLoader : public ITask
{
    BSPLevel *m_pLevel;
    BSPWorld *m_pWorld;

public:
    BSPAsynchLoader(BSPWorld *pWorld, BSPLevel *level);

    ITaskStepResult *ExecuteStep(LoaderThread *loaderThread) override;
    void             OnCompletion() override;
};

class BSPLoaderTaskStepResult : public ITaskStepResult
{
public:
    BSPLoaderTaskStepResult(size_t nCurrentSteps, size_t nTotalSteps);
    ~BSPLoaderTaskStepResult();
};

class BSPLoaderCompletionStep : public ITaskStepResult
{
    BSPWorld *m_pLevel;

public:
    BSPLoaderCompletionStep(BSPWorld *pLevel);
    void ExecuteOnCompletion() override;

    bool NeedEndCallback() override;
};

} // namespace GoldSource
