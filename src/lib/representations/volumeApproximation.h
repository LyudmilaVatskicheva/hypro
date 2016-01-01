/**
 *
 */

#include "../config.h"

namespace hypro {

	template<typename Number, typename Representation>
	static Number approximateVolume(Representation _in, std::size_t sublevels = 0) {
		Number pointCount = 0;
		std::vector<Point<Number>> vertices = _in.vertices();

		//for(Point<Number> vertex: vertices) {
		//	std::cout << "Vertex of new poyltope:\n" << vertex.coordinate(0) << ", " << vertex.coordinate(1) << ", " << vertex.coordinate(2) << std::endl;
		//}

		unsigned dimension = vertices[0].dimension();
		std::vector<std::pair<Number,Number>> bounderies;
		std::vector<bool> count_help;
		std::vector<Number> count_point;
		std::vector<Number> resolution;
		bool running=true;
		Number volumeUnit=1;

		// TODO, also figure out, if we need Number as a template parameter (I guess not)
		// TODO: where do we set the initial resolution? As a parameter with a default value specified as a constant in config.h maybe.

		// init
		for(unsigned i = 0; i<dimension; i++){
			bounderies.push_back(std::pair<Number,Number>(0,0));
			count_help.push_back(false);
			count_point.push_back(0);
			resolution.push_back(0);
		}

		// Compute bounderies
		for(Point<Number> vertex: vertices) {
			for(unsigned i = 0; i<dimension; i++){
				if(bounderies.at(i).first>vertex.coordinate(i)) { // min
					bounderies.at(i).first = vertex.coordinate(i);
				}
				else if(bounderies.at(i).second<vertex.coordinate(i)) { // max
					bounderies.at(i).second = vertex.coordinate(i);
				}
			}
		}

		// Post-init with bounderies-info: compute resolution, volumeUnit and init count_point
		for(unsigned i = 0; i<dimension; i++ ) {
		  //std::cout << "bound." << i << " : " << bounderies[i].first << " till " << bounderies[i].second << std::endl;
			resolution[i]= (bounderies[i].second-bounderies[i].first)/25; // 100 for 2D, 50 for 3D, 12 for 4D - if to high -> might be very slow
			if(!carl::AlmostEqual2sComplement(resolution[i]+(Number)1,(Number) 1)){
				volumeUnit*=resolution[i];
			}
			count_point[i]=bounderies[i].first;
	}

		// Create a grid from bounderies and decide (count) for each point if _in contains it - recursive?
		while(running){
			for(unsigned i=0; i<dimension; i++){
				if(i==0 || count_help[i]){
						count_point[i]+=resolution[i];
						count_help[i]=false;
						if(count_point[i]>bounderies[i].second && i<dimension-1){
							count_point[i]=bounderies[i].first;
							count_help[i+1]=true;
						} else if(count_point[i]>bounderies[i].second && i==dimension-1){
							running=false;
						}
				}
			}
			// Point computed
			//std::cout << "count_point (";
			//for(unsigned i = 0; i<dimension; i++ ) {
			//	std::cout << count_point[i] << " r"<<resolution[i] << " ";
			//}
			//std::cout << ") is ";
			if(_in.contains(count_point)){
				pointCount+=volumeUnit;
				//std::cout << " inside!" << std::endl;
			} else {
				//std::cout << " NOT inside!" << std::endl;
			}
		}

		return pointCount;
	}

}
