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
 *
 * ## Quick Guide for Selection:
 *
 * 1. Is it a function? -> `CX_FN`
 * 2. Is it a lambda? -> `CX_EXEC`
 * 3. Is it in a header? -> `CX_INLINE`
 * 4. Is it in a function/test? -> `CX_CONST`
 * 5. Does it define an array size? -> `CX_VALUE`
 */
#ifdef COVERAGE_BUILD

/**
 * @brief Logic: Methods and Functions.
 * * USE CASE: Use for any function or class method containing bitwise logic,
 * search algorithms, or constructors.
 * GOAL:     Forces the compiler to generate a real function call with its own
 * entry point so coverage tools can track hits/misses.
 * COVERAGE: Becomes 'inline __attribute__((used))'.
 * LINKAGE:  External (ODR-compliant inline).
 */
#define CX_FN inline __attribute__((used))

/**
 * @brief Shared Data: Global or Namespace-level constants.
 * * USE CASE: Use for variables in headers or namespaces that are shared across
 * multiple .cpp files (e.g., Piece constants, shared lookup tables).
 * GOAL:     Downgrades to a runtime-initialized 'inline const' to track the
 * initialization logic while preventing "multiple definition" errors.
 * COVERAGE: Becomes 'inline const'.
 * LINKAGE:  External (Inline linkage).
 */
#define CX_INLINE inline const

/**
 * @brief Local Data: Variables inside functions or tests.
 * * USE CASE: Use for temporary constants inside a function body or a TEST() block.
 * GOAL:     Allows the variable to be treated as a standard 'const' at runtime.
 * Crucial because the 'inline' keyword is illegal inside block scopes.
 * COVERAGE: Becomes 'const'.
 * LINKAGE:  None (Block scope / Local).
 */
#define CX_CONST const

/**
 * @brief Execution: Lambda specifiers.
 * * USE CASE: Use in lambda declarations: []() CX_EXEC { ... }.
 * GOAL:     Marks the lambda as constexpr-compatible in Release, but allows
 * it to be instrumented as a standard function in Coverage.
 * COVERAGE: Becomes empty (standard runtime lambda).
 * LINKAGE:  Internal to the lambda object.
 */
#define CX_EXEC

/**
 * @brief Structural: Core compiler constants.
 * * USE CASE: Use for array sizes, template parameters, or 'formatter::parse'
 * where the compiler REQUIRES a constant expression to build.
 * GOAL:     Remains 'constexpr' even in Coverage builds to ensure the
 * project still compiles. No coverage is tracked for these.
 * COVERAGE: Stays 'constexpr'.
 * LINKAGE:  Internal/External (Standard constexpr rules).
 */
#define CX_VALUE constexpr

/**
 * @brief Validation: Compile-time vs Runtime assertions.
 * * USE CASE: Use to verify that logic is constexpr-compatible.
 * GOAL:     Uses 'static_assert' in Release to ensure zero-cost logic, but
 * shifts to 'EXPECT_TRUE' in Coverage so non-constexpr code can pass.
 * COVERAGE: Becomes 'EXPECT_TRUE(expr)'.
 */
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
