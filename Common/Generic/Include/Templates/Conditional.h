#pragma once

/*
 * std::conditional
 */

template <bool Predicate, typename LHS, typename RHS>
struct TConditional;

template <typename LHS, typename RHS>
struct TConditional<true, LHS, RHS>
{
	typedef LHS Type;
};

template <typename LHS, typename RHS>
struct TConditional<false, LHS, RHS>
{
	typedef RHS Type;
};
