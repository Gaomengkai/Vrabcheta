//
// Created by gaome on 2023/7/19.
//

#ifndef IRBACKENDR5_IROPTIMIZER_H
#define IRBACKENDR5_IROPTIMIZER_H

#include "IROptimizerBase.h"
#include "NoneOptimizer.h"
#include "IROptRLE.h"
#include "IROptDCE1.h"
#include "IROptCAFP.h"
#include "IROptCSLR.h"

namespace MiddleIR::Optimizer
{
class IROptimizer
{
public:
    enum ENABLED_OPT : uint64_t {
        O0                               = 0x0,
        CONSTANT_FOLDING                 = 0x1,
        CONSTANT_PROPAGATION             = 0x2,
        COPY_PROPAGATION                 = 0x4,
        DEAD_CODE_ELIMINATION            = 0x8,
        COMMON_SUBEXPRESSION_ELIMINATION = 0x10,
        LOOP_OPTIMIZATION                = 0x20,
        FUNCTION_INLINE                  = 0x40,
        NONE_OPTIMIZATION                = 0x80,
        REDUNDANT_LOAD_ELIMINATION       = 0x100,
        INSTRUCTION_COMBINE              = 0x200,
        CAFP                             = 0x400,
        CSLR                             = 0x800,
        ALL                              = (uint64_t)-1
    } enabledOpt           = O0;
    virtual ~IROptimizer() = default;
    explicit IROptimizer(const shared_ptr<MiddleIRAST>& irast_, ENABLED_OPT enabledOpt_ = O0)
        : _irast(irast_)
        , enabledOpt(enabledOpt_)
    {
        //        if (NONE_OPTIMIZATION & enabledOpt) { _optimizers.push_back(new
        //        NoneOptimizer(irast_)); }
        if (REDUNDANT_LOAD_ELIMINATION & enabledOpt) {
            _optimizers.push_back(new IROptRLE(irast_));
        }
        if (DEAD_CODE_ELIMINATION & enabledOpt) { _optimizers.push_back(new IROptDCE1(irast_)); }
        if (CAFP & enabledOpt) { _optimizers.push_back(new IROptCAFP(irast_)); }
        if (CSLR & enabledOpt) { _optimizers.push_back(new IROptCSLR(irast_)); }
    }
    virtual void run()
    {
        for (auto& optimizer : _optimizers) { optimizer->run(); }
    }

protected:
    shared_ptr<MiddleIRAST>  _irast;
    vector<IROptimizerBase*> _optimizers;
};
}   // namespace MiddleIR::Optimizer

#endif   // IRBACKENDR5_IROPTIMIZER_H
