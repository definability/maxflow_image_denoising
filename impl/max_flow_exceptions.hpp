#ifndef MAXFLOW_IMAGE_DENOISING_MAX_FLOW_EXCEPTIONS_HPP
#define MAXFLOW_IMAGE_DENOISING_MAX_FLOW_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <utility>

class MaxFlowException : public std::exception
{
public:
  explicit MaxFlowException(std::string message)
    : message{std::move(message)}
  {
  }

  [[nodiscard]] const char* what() const noexcept override
  {
    return message.c_str();
  }

private:
  std::string message;
};

class EdgeInitialisationException : public MaxFlowException
{
public:
  explicit EdgeInitialisationException(std::string message)
    : MaxFlowException{std::move(message)}
  {
  }
};

class ResultConsistencyException : public MaxFlowException
{
public:
  explicit ResultConsistencyException(std::string message)
    : MaxFlowException{std::move(message)}
  {
  }
};

#endif //MAXFLOW_IMAGE_DENOISING_MAX_FLOW_EXCEPTIONS_HPP
