#include "max_flow_solver.hpp"

#include "impl/max_flow_solver_implementation.hpp"

#include <memory>

MaxFlowSolver::MaxFlowSolver(
  const ImageSize height,
  const ImageSize width,
  const DiscontinuityPenalty discontinuity_penalty)
  : implementation{
    std::make_unique<MaxFlowSolverImplementation>(
      height, width, discontinuity_penalty
    )
  }
{
}

void MaxFlowSolver::operator()(GreyscaleImage& noisy_image) const
{
  (*implementation)(noisy_image);
  (*implementation) >> noisy_image;
}

MaxFlowSolver::MaxFlowSolver(MaxFlowSolver&&) noexcept = default;

MaxFlowSolver& MaxFlowSolver::operator=(MaxFlowSolver&&) noexcept = default;

MaxFlowSolver::~MaxFlowSolver() = default;
