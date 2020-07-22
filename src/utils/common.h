#pragma once

#define DISALLOW_COPY_AND_MOVE(CLASS)                                          \
  CLASS(const CLASS &obj) = delete;                                            \
  CLASS &operator=(const CLASS &obj) = delete;                                 \
  CLASS(CLASS &&obj) noexcept = delete;                                        \
  CLASS &operator=(CLASS &&obj) noexcept = delete

#define TRY_INIT_WITH_EXPR(VAR, LAMBDA, FAIL_CODE)                             \
  try {                                                                        \
    (VAR) = (LAMBDA)();                                                        \
  } catch (...) {                                                              \
    return FAIL_CODE;                                                          \
  }

enum class ret_code_t {
  OK,
  ERROR
};