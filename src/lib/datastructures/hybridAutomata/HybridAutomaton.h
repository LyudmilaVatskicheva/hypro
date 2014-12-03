/* 
 * Class that describes a hybrid automaton.
 * File:   hybridAutomaton.h
 * Author: stefan & ckugler
 *
 * Created on April 30, 2014, 9:06 AM
 */

#pragma once

#include "Location.h"
#include "Transition.h"
#include "util.h"

namespace hypro
{
    template<typename Number>
    class HybridAutomaton
    {
        private:
			typedef hypro::Location<Number> location;
			typedef hypro::Transition<Number> transition;
			typedef std::set<location*> locationSet;
			typedef std::set<transition*> transitionSet;

        public:
			//hybrid automaton: a set of locations, a set of initial locations, a set of transitions and a valuation (currently a polytope)
    		struct hybridAutomaton {
    			locationSet init;
    			locationSet locs;
    			transitionSet trans;
    			hypro::valuation_t<Number> valuation;
    		};

        private:
    		/**
    		 * Member
    		 */
    		hybridAutomaton mHybridAutomaton;

        public:
    		/**
    		 * Constructors & Destructor
    		 */
    		HybridAutomaton(){}
    		HybridAutomaton(const HybridAutomaton& _hybrid);
    		HybridAutomaton(const locationSet _initLocs, const locationSet _locs, const transitionSet _trans, hypro::valuation_t<Number> _initVal);

    		virtual ~HybridAutomaton()
    		{}

    		/**
    		 * Getter & Setter
    		 */
    		locationSet initialLocations();
    		locationSet locations();
    		transitionSet transitions();
    		hypro::valuation_t<Number> valuation();
    		hybridAutomaton hybridAutomaton();

    		void setInitialLocations(locationSet _initLocs);
    		void setLocations(locationSet _locs);
    		void setTransitions(transitionSet _trans);
    		void setValuation(hypro::valuation_t<Number> _val);
    		void setHybridAutomaton(struct hybridAutomaton _hybrid);
			
			void addLocation(location* _location);
			void addTransition(transition* _transition);
			
			// move operator
			inline HybridAutomaton<Number>& operator= (HybridAutomaton<Number>&& _rhs);
    };
}

#include "HybridAutomaton.tpp"
