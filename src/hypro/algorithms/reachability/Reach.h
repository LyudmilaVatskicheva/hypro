/**
 * Reach holds a basic forward reachability analysis algorithm for linear hybrid automata.
 * @file Reach.h
 *
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 * @author Christopher Kugler
 * @author Phillip Florian
 *
 * @since	2014-04-30
 * @version	2016-06-20
 */

#pragma once
#include "FirstSegment.h"
#include "datastructures/HybridAutomaton/Settings.h"
#include "datastructures/HybridAutomaton/HybridAutomaton.h"
#include "datastructures/HybridAutomaton/State.h"
#include "config.h"
#include "util/Plotter.h"
#include "representations/Ellipsoids/Ellipsoid.h"
#include "representations/GeometricObject.h"
#include "boost/tuple/tuple.hpp"

CLANG_WARNING_DISABLE("-Wdeprecated-register")
#include <eigen3/unsupported/Eigen/src/MatrixFunctions/MatrixExponential.h>
CLANG_WARNING_RESET

// Debug Flag, TODO: Add more debug levels.
#define REACH_DEBUG
#define USE_REDUCTION
//#define USE_SYSTEM_SEPARATION
// Needs system separation to affect the computation
//#define USE_ELLIPSOIDS

#define USE_SMART_AGGREGATION
#define ALWAYS_USE_AGGREGATION
//#define USE_FORCE_REDUCTION

namespace hypro {
/**
 * \namespace reachability
 * \brief Namespace for all reachabiltiy analysis algorithm related code.
 */
namespace reachability {

template<typename Number>
using initialSet = boost::tuple<unsigned, State_t<Number>>;

template <typename Number>
using flowpipe_t = std::vector<State_t<Number>>;

#include "util.h"

/**
 * @brief      Class implementing a basic reachbility analysis algorithm for linear hybrid automata.
 *
 * @tparam     Number          The used number type.
 * @tparam     Representation  The used state set representation type.
 */
template <typename Number>
class Reach {
private:
	HybridAutomaton<Number, State_t<Number,Number>> mAutomaton;
	ReachabilitySettings<Number> mSettings;
	std::size_t mCurrentLevel = 0;
    Number mBloatingFactor = 0;
	std::map<unsigned, std::vector<flowpipe_t<Number>>> mReachableStates;
	std::list<initialSet<Number>> mWorkingQueue;
	Plotter<Number>& plotter = Plotter<Number>::getInstance();
	representation_name mType;

	mutable bool mIntersectedBadStates;

public:
	/**
	 * @brief Constructor for a basic reachability analysis algorithm for linear hybrid automata.
	 *
	 * @param _automaton The analyzed automaton.
	 * @param _settings The reachability analysis settings.
	 */
	Reach( const HybridAutomaton<Number, State_t<Number,Number>>& _automaton, const ReachabilitySettings<Number>& _settings = ReachabilitySettings<Number>());

	/**
	 * @brief Computes the forward reachability of the given automaton.
	 * @details
	 * @return The flowpipe as a result of this computation.
	 */
	std::vector<std::pair<unsigned, flowpipe_t<Number>>> computeForwardReachability();

	/**
	 * @brief Computes the forward time closure (FTC) of the given valuation in the respective location.
	 * @details [long description]
	 *
	 * @param _loc The location in which the FTC is to be computed.
	 * @param _val The valuation which is initial in the respective location.
	 *
	 * @return The id of the computed flowpipe.
	 */
	flowpipe_t<Number> computeForwardTimeClosure( const State_t<Number>& _state );


	/**
	 * @brief Returns whether the bad states were reachable so far.
	 * @details [long description]
	 * @return true, if the bad states were reachable.
	 */
	bool reachedBadStates() const { return mIntersectedBadStates; }

	/**
	 * @brief Computes one time step and one discrete step, i.e. increases the depth of the search by one.
	 * @details [long description]
	 *
	 * @param _init The initial valuations.
	 * @return The resulting flowpipes.
	 */
	void processDiscreteBehaviour( const std::vector<boost::tuple<Transition<Number>*, State_t<Number>>>& _newInitialSets );

	/**
	 * @brief Checks, whether the passed transition is enabled by the passed valuation. Sets the result to be the intersection of the guard and the valuation.
	 * @details [long description]
	 *
	 * @param _trans The transition, which is to be enabled.
	 * @param _val The valuation possibly enabling the passed transition.
	 * @param result At the end of the method this holds the result of the intersection of the guard and the valuation.
	 * @return True, if the transition is enabled, false otherwise.
	 */
	bool intersectGuard( Transition<Number>* _trans, const State_t<Number>& _segment, State_t<Number>& result ) const;

	bool checkTransitions(const State_t<Number>& _state, const carl::Interval<Number>& currentTime, std::vector<boost::tuple<Transition<Number>*, State_t<Number>>>& nextInitialSets) const;

	const ReachabilitySettings<Number>& settings() const { return mSettings; }
	void setSettings(const ReachabilitySettings<Number>& settings) { mSettings = settings; }

	representation_name getRepresentationType() const { return mType; }
	void setRepresentationType(const representation_name& type) { mType = type; }

private:

	matrix_t<Number> computeTrafoMatrix( const Location<Number>* _loc ) const;
	//boost::tuple<bool, State_t<Number>, matrix_t<Number>, vector_t<Number>> computeFirstSegment( const State_t<Number>& _state ) const;
	bool intersectBadStates( const State_t<Number>& _state ) const;
};

}  // namespace reachability
}  // namespace hypro

#include "Reach.tpp"
#include "discreteHandling.tpp"
#include "terminationHandling.tpp"

//#include "Reach_SF.h"
