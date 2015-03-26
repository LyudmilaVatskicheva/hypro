/*
 * Contains utilities to create the convex hull.
 * @file   convexHull_util.h
 * @author tayfun
 *
 * @since       2014-12-10
 * @version     2015-01-21
 */

#pragma once

#include "../../datastructures/Hyperplane.h"
#include "../../datastructures/Facet.h"
#include "../../datastructures/Ridge.h"
#include "../../datastructures/Point.h"

namespace hypro
{
namespace polytope
{

/*
 * Creates initial Convex Hull in dimension d with d+1 Points out of all the points given. Neighbor relationship of the facets are saved in neighbors.
 * @return The list of facets, i.e. Hyperplanes with dimension d-1 who form the Convex Hull.
 */
template<typename Number>
static std::vector<Facet<Number>> initConvexHull(const std::vector<Point<Number>>& points){
	// TODO: check if points is empty or contains less than d+1 elements!
	int d = points[0].dimension();
	std::cout << __func__ << " : " << __LINE__ << " : " << d << std::endl;
	std::vector<Point<Number>> initialPoints;
	Facet<Number> f = Facet<Number>();
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	for(int i = 0; i < d+1; ++i) {
		initialPoints.push_back(points[i]);
		// TODO: Introduce a consistency check here to ensure that this is not degenerate.
	}
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	std::vector<Facet<Number>> facets;
	for(int i = 0; i < d+1; i++) {
		facets.push_back(f);
	}
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	for(int i = 0; i < d+1; ++i) {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		std::vector<Point<Number>> points_for_facet;
		for(int j = 0; j < d+1; ++j) {
			if(i!=j) {
				points_for_facet.push_back(initialPoints[j]);
				// facets[i].addNeighbor(facets[j]);
			}
		}
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		facets[i].setPoints(points_for_facet, initialPoints[i]);
	}

	for(int i = 0; i < d+1; ++i) {
		for(int j = 0; j < d+1; ++j) {
			if(i!=j) {
				facets[i].addNeighbor(facets[j]);
			}
		}
	}

	std::cout << __func__ << " : " << __LINE__ << std::endl;
	return facets;
}

/*
 * @return The list of points that are not contained by the facets.
 */
template<typename Number>
static std::vector<Point<Number>> points_not_in_facets(std::vector<Point<Number>> points, const std::vector<Facet<Number>>& facets){
	std::vector<Point<Number>> pointsInFacets;
	for(Facet<Number> facet : facets) {
		std::vector<Point<Number>> pointsInFacet = facet.vertices();
		for(Point<Number> p : pointsInFacet) {
			pointsInFacets.push_back(p);
		}
	}
	// Stefan: pointsInFacets holds all generating points of the given facets.

	//std::cout << __func__ << " : " << __LINE__ << pointsInFacets << std::endl;
	std::vector<int> removeables;
	for (unsigned i = 0; i<points.size(); i++) {
		for (unsigned j = 0; j<pointsInFacets.size(); j++) {
			if(points[i] == pointsInFacets[j]) {
				removeables.push_back(i);
			}
		}
	}
	// Stefan: removables holds the indices of the points in points, which are contained in one of the facets.

	//std::cout << __func__ << " : " << __LINE__ << removeables << std::endl;
	std::vector<int> remove;
	for(unsigned i = 0; i<removeables.size(); i++){
		bool found = false;
		for(unsigned j = 0; j<remove.size(); j++){
			if(remove[j] == removeables[i]){
				found = true;
			}
		}
		if(!found){
			remove.push_back(removeables[i]);
		}
	}
	// Clear removables of duplicates (-> remove)

	for(unsigned i = 0; i<remove.size(); i++){
		points.erase(points.begin() + remove[i]-i);
	}

	std::cout << __func__ << " : result points: " << __LINE__ << points << std::endl;
	return points;
}

/*
 * Determines the neighbors of the Hyperplanes. Neighbors are taken from neighbors.
 * @return The list of neighbors from the Hyperplanes.
 */
template<typename Number>
static std::vector<Facet<Number>> getFacetsNeighbors(const std::vector<Facet<Number>>& facets, std::vector<Facet<Number>> allowed){
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	if(facets.empty()) {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		return std::vector<Facet<Number>>();
	}
	else {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		std::vector<Facet<Number>> result;
		std::vector<Facet<Number>> temp;
		std::vector<Facet<Number>> temp2;
		std::vector<Facet<Number>> neighbors;
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		for(unsigned i = 0; i<facets.size(); i++) {
			neighbors = facets[i].neighbors();
			for(unsigned j = 0; j<neighbors.size(); j++) {
				temp.push_back(neighbors[j]);
			}
		}
		// Stefan: After this, temp contains all collected neighbors of all input facets.

		std::cout << __func__ << " : " << __LINE__  << temp << std::endl;
		bool found = false;
		for(unsigned i = 0; i<temp.size(); i++) {
			for(unsigned j = 0; j<temp2.size(); j++) {
				if(temp[i] == temp2[j]) {
					found = true;
				}
			}
			if(!found) {
				temp2.push_back(temp[i]);
			}
			found = false;
		}
		// Stefan: Here temp2 is a copy of temp, where the duplicates are removed.

		std::cout << __func__ << " : " << __LINE__ << std::endl;
		found = false;
		for(unsigned i = 0; i<temp2.size(); i++) {
			for(unsigned j = 0; j<facets.size(); j++) {
				if(temp2[i] == facets[j]) {
					found = true;
				}
			}
			if(!found) {
				result.push_back(temp2[i]);
			}
			found = false;
		}
		// Stefan: result contains all facets, that are neighbors of the input facets (without duplicates and without the facets themselves).
		std::vector<int> remove;
		for(unsigned j = 0; j<allowed.size(); j++){
			for(unsigned i = 0; i < result.size(); i++){
				//if(result[i]==allowed[j]) {
				//	remove.push_back(i);
				//}
				result.erase(result.begin() + i);
			}
		}

		//for(unsigned i = 0; i < remove.size(); i++){
		//	result.erase(result.begin() + remove[i] - i);
		//}

		std::cout << __func__ << " : result: " << result << std::endl;
		return result;
	}
}
template<typename Number>
static std::vector<Facet<Number>> getFacetsNeighbors2(const std::vector<Facet<Number>>& facets){
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	if(facets.empty()) {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		return std::vector<Facet<Number>>();
	}
	else {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		std::vector<Facet<Number>> result;
		std::vector<Facet<Number>> temp;
		std::vector<Facet<Number>> temp2;
		std::vector<Facet<Number>> neighbors;
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		for(unsigned i = 0; i<facets.size(); i++) {
			neighbors = facets[i].neighbors();
			for(unsigned j = 0; j<neighbors.size(); j++) {
				temp.push_back(neighbors[j]);
			}
		}
		// Stefan: After this, temp contains all collected neighbors of all input facets.

		std::cout << __func__ << " : " << __LINE__  << temp << std::endl;
		bool found = false;
		for(unsigned i = 0; i<temp.size(); i++) {
			for(unsigned j = 0; j<temp2.size(); j++) {
				if(temp[i] == temp2[j]) {
					found = true;
				}
			}
			if(!found) {
				temp2.push_back(temp[i]);
			}
			found = false;
		}
		// Stefan: Here temp2 is a copy of temp, where the duplicates are removed.

		std::cout << __func__ << " : " << __LINE__ << std::endl;
		found = false;
		for(unsigned i = 0; i<temp2.size(); i++) {
			for(unsigned j = 0; j<facets.size(); j++) {
				if(temp2[i] == facets[j]) {
					found = true;
				}
			}
			if(!found) {
				result.push_back(temp2[i]);
			}
			found = false;
		}
		// Stefan: result contains all facets, that are neighbors of the input facets (without duplicates and without the facets themselves).

		std::cout << __func__ << " : result: " << result << std::endl;
		return result;
	}
}
/*
 * Determines the ridges between the facets and their neighbors.
 * @return The list of ridges, i.e. Hyperplanes with dimension d-2 who are the intersection of two facets.
 */
template<typename Number>
static std::vector<Ridge<Number>> getRidges(const std::vector<Facet<Number>>& facets) {
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	if(facets.empty()) {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		return std::vector<Ridge<Number>>();
	}
	else {
		std::cout << __func__ << " : " << __LINE__ << std::endl;
		std::vector<Ridge<Number>> result;
		std::vector<Facet<Number>> neighbors = getFacetsNeighbors2(facets);
		std::cout << __func__ << " : " << __LINE__ << "neighbors: " << neighbors << std::endl;
		for(unsigned i = 0; i<facets.size(); ++i) {
			for(unsigned j = 0; j<neighbors.size(); ++j) {
				if(facets[i].isNeighbor(neighbors[j])) {
					std::cout << "facets[i].isNeighbor(neighbors[j]): " << neighbors[j] << " is neighbor of " << facets[i]  << std::endl;
					Ridge<Number> newRidge(facets[i], neighbors[j]);
					std::cout << "Ping" << std::endl;
					result.push_back(newRidge);
				}
			}
		}
		std::cout << __func__ << " :  result " << result << std::endl;
		return result;
	}
}




/*
 * Determines the list of points which were outside of the visible_facets. The points are taken from hashlist.
 * @return The list of points which were outside of the visible_facets
 */
template<typename Number>
static std::vector<Point<Number>> points_outside_of_visible_facets(const std::vector<Facet<Number>>& visible_facets) {
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	std::vector<Point<Number>> temp;
	std::vector<Point<Number>> result;
	std::vector<Point<Number>> outsideset;
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	for(unsigned i = 0; i < visible_facets.size(); i++) {
		outsideset = visible_facets[i].getOutsideSet();
		for(unsigned j = 0; j<outsideset.size(); j++) {
			temp.push_back(outsideset[j]);
		}
	}
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	bool found = false;
	for(unsigned i = 0; i < temp.size(); i++) {
		for(unsigned j = 0; i<result.size(); j++) {
			if(temp[i] == result[j]) {
				found = true;
			}
		}
		if(!found) {
			result.push_back(temp[i]);
		}
		found = false;
	}
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	return result;
}

/*
 * Checks for each facet if it has at least one point above. If it has it is not outside and will be returned.
 * @return The list of facets that have at least one point above.
 */
template<typename Number>
static std::queue<Facet<Number>> not_outside_facet(const std::vector<Facet<Number>>& facets) {
	std::cout << __func__ << " : " << __LINE__ << facets << std::endl;
	std::cout << __func__ << " : " << __LINE__ << facets[0].getOutsideSet() << std::endl;
	std::queue<Facet<Number>> result;
	for(unsigned i = 0; i < facets.size(); i++) {
		std::cout << __func__ << " : " << __LINE__ << facets[i].getOutsideSet() << std::endl;
		if(!facets[i].getOutsideSet().empty()) {
			result.push(facets[i]);
		}
	}
	//std::cout << __func__ << " : " << __LINE__ << result << std::endl;
	return result;
}

template<typename Number>
static bool neighborCheck(const Facet<Number> facet1, const Facet<Number> facet2) {
	if(facet1 == facet2){
		return false;
	}
	else {
		int checkValue = facet1.vertices().size() - 1;
		int currentValue = 0;
		for(unsigned i = 0; i < facet1.vertices().size(); i++) {
			for(unsigned j = 0; j < facet2.vertices().size(); j++){
				if(facet1.vertices().at(i) == facet2.vertices().at(j)){
					currentValue++;
				}
			}
		}
		return checkValue == currentValue;
	}
}

template<typename Number>
static void determineNeighbors(std::vector<Facet<Number>>& newFacets){
	for(unsigned i = 0; i < newFacets.size(); i++) {
		for(unsigned j = 0; j < newFacets.size(); j++){
			if(neighborCheck(newFacets[i],newFacets[j])){
				newFacets[i].addNeighbor(newFacets[j]);
			}
		}
	}
}

template<typename Number>
static Point<Number> findInsidePoint(const Ridge<Number>& ridge, const std::vector<Facet<Number>>& visible_facets){
	for(Facet<Number> facet : visible_facets){
		for(Point<Number> point1 : facet.vertices()){
			Point<Number> result;
			bool once = false;
			bool twice = false;
			for(Point<Number> point2: ridge.vertices()){
				if(point1 != point2 && !once){
					result = point1;
					once = true;
				}
				if(point1 != point2 && !twice){
					result = point1;
					twice = true;
				}
			}
			if(!twice && once){
				return result;
			}
		}
	}
	return Point<Number>();
}

}

}
