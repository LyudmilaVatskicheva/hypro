/**
 * Specialization for a converter to an H-polytope.
 * @file converterToHPolytope.h
 * @author Simon Froitzheim
 *
 * @since	2015-12-17
 * @version	2015-12-17
 */

#pragma once

namespace hypro {
    /**
     * @brief Converts a H-Polytope into another H-Polytope.
     * @detail Simply overwrites _target with _source.
     *   
     * @param _source H-Polytope that needs to be converted.
     * @param _target An arbitrary H-Polytope that gets overwritten with the converted object.
     * @return Return value is true if the conversion process was successful.
     */  
    template <typename Number>
    static bool convert( const hypro::HPolytope<Number>& _source, hypro::HPolytope<Number>& _target );
    
    /**
     * @brief Converts a polytope in V-representation into an H-Polytope.
     * 
     *   
     * @param _source V-polytope that needs to be converted.
     * @param _target An arbitrary H-Polytope that gets overwritten with the converted object.
     * @return Return value is true if the conversion process was successful.
     */  
    template <typename Number>
    static bool convert( const hypro::VPolytope<Number>& _source, hypro::HPolytope<Number>& _target );
    
    /**
     * @brief Converts a box into an H-Polytope.
     * 
     *   
     * @param _source Box that needs to be converted.
     * @param _target An arbitrary H-Polytope that gets overwritten with the converted object.
     * @return Return value is true if the conversion process was successful.
     */  
    template <typename Number>
    static bool convert( const hypro::Box<Number>& _source, hypro::HPolytope<Number>& _target );
    
    /**
     * @brief Converts a zonotope into an H-Polytope.
     * 
     *   
     * @param _source Zonotope that needs to be converted.
     * @param _target An arbitrary H-Polytope that gets overwritten with the converted object.
     * @return Return value is true if the conversion process was successful.
     */  
    template <typename Number>
    static bool convert( const hypro::Zonotope<Number>& _source, hypro::HPolytope<Number>& _target );   

}  // namespace

#include "converterToHPolytope.tpp"
