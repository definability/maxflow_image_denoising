#ifndef MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_HPP
#define MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_HPP

#include <memory>

#include "types.hpp"

class GreyscaleImage;

class MaxFlowSolver
{
public:
  MaxFlowSolver(
    ImageSize height,
    ImageSize width,
    DiscontinuityPenalty discontinuity_penalty);

  MaxFlowSolver(const MaxFlowSolver&) = delete;

  MaxFlowSolver(MaxFlowSolver&&) noexcept;

  MaxFlowSolver& operator=(const MaxFlowSolver&) = delete;

  MaxFlowSolver& operator=(MaxFlowSolver&&) noexcept;

  void operator()(GreyscaleImage& noisy_image) const;

  ~MaxFlowSolver();

private:
  class MaxFlowSolverImplementation;

  std::unique_ptr<MaxFlowSolverImplementation> implementation;
};

#endif //MAXFLOW_IMAGE_DENOISING_MAX_FLOW_SOLVER_HPP
