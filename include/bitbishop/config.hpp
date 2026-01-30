#pragma once

/**
 * @brief Qualification macros for constexpr-heavy code with optional runtime instrumentation.
 *
 * @warning These macros must be used everywhere a `constexpr` would normally be used.
 *          Direct use of the `constexpr` keyword is forbidden in this project.
 *
 * @note The project deliberately replaces all direct uses of `constexpr` with
 *       dedicated macros to avoid mixing raw language keywords with build-mode-
 *       dependent behavior. This enforces a strict, uniform rule:
 *       either the abstraction is used everywhere, or not at all.
 *
 * These macros provide a controlled mechanism to selectively relax `constexpr`
 * semantics when building with code coverage enabled, while preserving full
 * compile-time guarantees in standard builds.
 *
 * ## Motivation
 *
 * **Problem**
 * Variables and functions declared `constexpr` are fully evaluated at compile
 * time. Consequently, Clang emits no runtime initialization or execution code,
 * causing the associated logic to be invisible to LLVM coverage tools
 * (`llvm-cov` reports 0% coverage).
 *
 * **Approach**
 * When `COVERAGE_BUILD` is enabled, `constexpr` constructs are intentionally
 * downgraded to runtime equivalents (`const`, `inline`, or empty), forcing
 * code generation and allowing LLVM instrumentation to observe and count
 * executions.
 *
 * ## Notes
 * - This system exists solely to support accurate and meaningful coverage metrics.
 * - Engine correctness must never depend on coverage mode.
 * - The added complexity is intentionally isolated behind these macros to avoid
 *   leaking build-mode concerns into core engine logic.
 */
#ifdef COVERAGE_BUILD

/// Marks formerly constexpr functions as inline and "used" to force runtime emission.
#define CX_FN inline __attribute__((used))

/// Downgrades constexpr declarations to inline const for runtime instrumentation.
#define CX_INLINE inline const

/// Downgrades constexpr objects to const to force runtime initialization.
#define CX_CONST const

/// Removes constexpr in execution-only contexts (e.g. lambdas).
#define CX_EXEC

/// Always preserves constexpr for values that must remain constant expressions.
#define CX_VALUE constexpr

/// Replaces static_assert with a runtime test in coverage builds.
#define VALIDATE_CX(expr) EXPECT_TRUE(expr)

#else

/// Normal build: preserve full constexpr semantics.
#define CX_FN constexpr
#define CX_INLINE constexpr
#define CX_CONST constexpr
#define CX_EXEC constexpr
#define CX_VALUE constexpr
#define VALIDATE_CX(expr) static_assert(expr)

#endif
