/* 
 * This file defines the parser for different input files.
 * @file   parser.h
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since   2014-05-14
 * @version 2014-11-27
 */

#pragma once

#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/phoenix/stl/container.hpp>
#include "boost/variant.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "../datastructures/hybridAutomata/HybridAutomaton.h"
#include "utils.h"
#include "componentParser.h"
#include "../util/types.h"
#include "helper.h"

namespace hypro{
namespace parser{
    
namespace spirit = boost::spirit;    
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace fusion = boost::fusion;

typedef spirit::istream_iterator BaseIteratorType;
typedef spirit::line_pos_iterator<BaseIteratorType> PositionIteratorType;
typedef PositionIteratorType Iterator;
typedef qi::space_type Skipper;
typedef std::vector<boost::variant<Initial, State, Transition> > Automaton;


template<typename Iterator>
struct InitialParser : public qi::grammar<Iterator, Initial(), Skipper>
{
	shortLocationParser<Iterator> mLocationParser;
	
    InitialParser() : InitialParser::base_type(start)
    {
        start = qi::lit("initial") > 
				qi::lit("(") >
				mLocationParser >
				qi::lit(")");
    }
    
    qi::rule<Iterator, Initial(), Skipper> start;
};

template<typename Iterator>
struct StateParser : public qi::grammar<Iterator, State(), Skipper>
{
    FlowParser<Iterator> mFlowParser;
	InvariantParser<Iterator> mInvariantParser;
	NameParser<Iterator> mNameParser;
    
    StateParser() : StateParser::base_type(start)
    {
       start = qi::lit("location") >
			   qi::lit("(") > 
			   mNameParser >
			   -(qi::lit(",") > 
			   mFlowParser) >
			   -(qi::lit(",") > 
			   mInvariantParser) >
			   qi::lit(")");
    }
    
    qi::rule<Iterator, State(), Skipper> start;
};

template<typename Iterator>
struct TransitionParser : public qi::grammar<Iterator, Transition(), Skipper>
{
	NameParser<Iterator> mNameParser;
    GuardParser<Iterator> mGuardParser;
	ResetParser<Iterator> mResetParser;
    
    TransitionParser(): TransitionParser::base_type(start)
    {
        start = qi::lit("transition") >
				qi::lit("(") > -(qi::int_)  >
				-(mNameParser > qi::lit(",")) >
				qi::lexeme[(qi::lit("s") | qi::lit("S") ) > qi::lit("ource")] > qi::lit("=") > qi::uint_ > qi::lit(",") > 
				qi::lexeme[(qi::lit("t") | qi::lit("T") ) > qi::lit("arget")] > qi::lit("=") > qi::uint_ >
				-(qi::lit(",") > 
				mGuardParser) >
				-(qi::lit(",") > 
				mResetParser) >
				qi::lit(")");
    }
    
    qi::rule<Iterator, Transition(), Skipper> start;
};


// TODO: Template this!
struct MainParser : public qi::grammar<Iterator, Skipper>
{
	qi::rule<Iterator, Skipper> main;
    StateParser<Iterator> mStateParser;
    TransitionParser<Iterator> mTransitionParser;
	InitialParser<Iterator> mInitialParser;
	
	std::vector<State> mStates;
	std::vector<Transition> mTransitions;
	Initial mInitial;
    
    MainParser() : MainParser::base_type(main)
    {
        //using phoenix::construct;
        using phoenix::val;
        
        //main = -(mInitialParser) < *(mStateParser | mTransitionParser);
        //main = mInitialParser[mInitial(spirit::_val)] > *(mStateParser[ boost::phoenix::push_back(mStates, parser::State(spirit::_1)) ] | mTransitionParser[ boost::phoenix::push_back(mTransitions, spirit::_1) ]);
        main = mInitialParser[ phoenix::bind(&hypro::parser::MainParser::setInitial, phoenix::ref(*this), qi::_1)] > *(mStateParser[ phoenix::bind(&hypro::parser::MainParser::push_back_State, phoenix::ref(*this), qi::_1) ] | mTransitionParser[ phoenix::bind(&hypro::parser::MainParser::push_back_Transition, phoenix::ref(*this), qi::_1) ]);
       
		
		qi::on_error<qi::fail>
        (
        main
        , std::cout
            << val("Error! Expecting ")
            << qi::_4                               // what failed?
            << val(" here: \"")
            //<< boost_optional_detail::construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
            << val("\"")
            << std::endl
        );
    }    
	
	void push_back_State(const State& _in)
	{
		mStates.push_back(_in);
	}
	
	void push_back_Transition(const Transition& _in)
	{
		mTransitions.push_back(_in);
	}
	
	void setInitial(const Initial& _in)
	{
		mInitial = _in;
	}
    
    void parseInput(const std::string& pathToInputFile);
    bool parse(std::istream& in, const std::string& filename, HybridAutomaton<double>& _result);
	
	HybridAutomaton<double> createAutomaton();
};


class Automaton_visitor
    : public boost::static_visitor<>
{
public:

    State& operator()(State& i) const
    {
        return i;
    }

    Transition& operator()(Transition& i) const
    {
        return i;
    }
};

std::ostream& operator<<(std::ostream& lhs, const Automaton& rhs)
{
    std::vector<const State*> states;
    std::vector<const Transition*> transitions;
    std::cout << "Size: " << rhs.size() << std::endl;
    for(auto& item : rhs)
    {
        /*if(const State* i = boost::get<State>(&item))
        {
            std::cout << "state..." << std::endl;
            states.insert(states.end(), i);
        }
        if(const Transition* i = boost::get<Transition>(&item))
        {
            std::cout << "Transition..." << std::endl;
            transitions.insert(transitions.end(), i);
        }*/
       lhs << item << std::endl; 
    }
    /*for(auto& state : states)
    {
        lhs << state;
    }
    for(auto& transition : transitions)
    {
        lhs << transition;
    }*/
    return lhs;
}


}
}