/*
 * This file containts the implementation of support functions. It requires the concrete
 * implementation of set representations as support functions for a successful evaluation
 * as it implements the chain of operations created during reachability analysis.
 * @file SupportFunctionContent.h
 *
 * @author Norman Hansen
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @version	2015-02-27
 */

#pragma once

#include "config.h"
#include "util.h"
#include "PolytopeSupportFunction.h"
#include "BallSupportFunction.h"
#include "EllipsoidSupportFunction.h"
#include "util/templateDirections.h"

//#define SUPPORTFUNCTION_VERBOSE
//#define MULTIPLICATIONSUPPORTFUNCTION_VERBOSE
#define USE_LIN_TRANS_REDUCTION

namespace hypro {
template <typename Number>
class SupportFunctionContent;

template <typename Number>
struct sumContent {
	std::shared_ptr<SupportFunctionContent<Number>> lhs;
	std::shared_ptr<SupportFunctionContent<Number>> rhs;
	sumContent( const std::shared_ptr<SupportFunctionContent<Number>>& _lhs, const std::shared_ptr<SupportFunctionContent<Number>>& _rhs )
		: lhs( _lhs ), rhs( _rhs ) {}
	sumContent( const sumContent<Number>& _origin ) : lhs( _origin.lhs ), rhs( _origin.rhs ) {}
};

template <typename Number>
struct trafoContent {
	std::shared_ptr<SupportFunctionContent<Number>> origin;
	std::shared_ptr<const lintrafoParameters<Number>> parameters;
	unsigned currentExponent;
	std::size_t successiveTransformations;
	// 2^power defines the max. number of successive lin.trans before reducing the SF

	trafoContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, const matrix_t<Number>& A, const vector_t<Number>& b )
		: origin( _origin ), currentExponent(1) {
		// Determine, if we need to create new parameters or if this matrix and vector pair has already been used (recursive).
		parameters = std::make_shared<const lintrafoParameters<Number>>(A,b);
		// in case this transformation has already been performed, parameters will be updated.
		_origin->hasTrafo(parameters, A, b);
#ifdef USE_LIN_TRANS_REDUCTION
		// best points for reduction are powers of 2 thus we only use these points for possible reduction points
		bool reduced;
		do {
			reduced = false;
			if ( (origin->type() == SF_TYPE::LINTRAFO) && (*origin->linearTrafoParameters()->parameters == *parameters) && origin->linearTrafoParameters()->currentExponent == currentExponent ) {
				successiveTransformations = origin->linearTrafoParameters()->successiveTransformations +1 ;
			} else {
				successiveTransformations = 0;
			}
			//std::cout << "successiveTransformations with exponent " << currentExponent << ": " << successiveTransformations << std::endl;
			if (successiveTransformations == unsigned(carl::pow(2,parameters->power)-1)) {
				reduced = true;
				currentExponent = currentExponent*(carl::pow(2,parameters->power));
				for(std::size_t i = 0; i < unsigned(carl::pow(2,parameters->power)-1); i++ ){
					origin = origin->linearTrafoParameters()->origin;
				}
				// Note: The following assertion does not hold in combination with the current reduction techniques.
				//assert(origin->type() != SF_TYPE::LINTRAFO || (origin->linearTrafoParameters()->parameters == this->parameters && origin->linearTrafoParameters()->currentExponent >= currentExponent) );
			}
		} while (reduced == true);
		assert(origin->checkTreeValidity());
#endif
	}

	trafoContent( const trafoContent<Number>& _origin ) : origin( _origin.origin ), parameters(_origin.parameters), currentExponent(_origin.currentExponent), successiveTransformations( _origin.successiveTransformations )
	{}

	std::pair<matrix_t<Number>, vector_t<Number>> reduceLinTrans(const matrix_t<Number>& _a, const vector_t<Number>& _b, std::size_t _power){
		std::size_t powerOfTwo = carl::pow(2, _power);
			// first compute the new b
		vector_t<Number> bTrans = _b;
		matrix_t<Number> aTrans = _a;
		for (std::size_t i = 1; i < powerOfTwo ; i++){
			bTrans = _a*bTrans + _b;
		}
		// now compute a^i efficiently
		for (std::size_t i = 0; i < _power; i++){
			aTrans = aTrans*aTrans;
		}
		return std::make_pair(aTrans, bTrans);
	}
};

template <typename Number>
struct scaleContent {
	std::shared_ptr<SupportFunctionContent<Number>> origin;
	Number factor;
	scaleContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, Number _factor )
		: origin( _origin ), factor( _factor ) {}
	scaleContent( const scaleContent<Number>& _origin ) : origin( _origin.origin ), factor( _origin.factor ) {}
};

template <typename Number>
struct unionContent {
	std::vector<std::shared_ptr<SupportFunctionContent<Number>>> items;
	unionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _lhs, const std::shared_ptr<SupportFunctionContent<Number>>& _rhs )
	{
		// This constructor is legacy.
		items.push_back(_lhs);
		items.push_back(_rhs);
		assert(items.size() == 2);
	}

	unionContent( const std::vector<std::shared_ptr<SupportFunctionContent<Number>>>& sfVector ) : items(sfVector) {}
	unionContent( const unionContent<Number>& _origin ) = default;
};

template <typename Number>
struct intersectionContent {
	std::shared_ptr<SupportFunctionContent<Number>> lhs;
	std::shared_ptr<SupportFunctionContent<Number>> rhs;
	intersectionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _lhs, const std::shared_ptr<SupportFunctionContent<Number>>& _rhs )
		: lhs( _lhs ), rhs( _rhs ) {}
	intersectionContent( const intersectionContent<Number>& _origin ) = default;
};

template<typename Number>
struct projectionContent {
	std::shared_ptr<SupportFunctionContent<Number>> origin;
	std::vector<unsigned> dimensions;
	projectionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, const std::vector<unsigned>& _dimensions )
		: origin(_origin), dimensions(_dimensions) {}
	projectionContent( const projectionContent<Number>& _original ) : origin(_original.origin), dimensions(_original.dimensions) {}
};

/**
 * @brief      Class for support function content.
 * @tparam     Number  The used number type.
 */
template <typename Number>
class SupportFunctionContent {
	friend trafoContent<Number>;

  private:
	SF_TYPE mType = SF_TYPE::NONE;
	unsigned mDepth;
	unsigned mOperationCount;
	unsigned mDimension;
	bool mContainsProjection;
	union {
		sumContent<Number>* mSummands;
		trafoContent<Number>* mLinearTrafoParameters;
		scaleContent<Number>* mScaleParameters;
		unionContent<Number>* mUnionParameters;
		intersectionContent<Number>* mIntersectionParameters;
		projectionContent<Number>* mProjectionParameters;
		PolytopeSupportFunction<Number>* mPolytope;
		BallSupportFunction<Number>* mBall;
		EllipsoidSupportFunction<Number>* mEllipsoid;
	};

	std::weak_ptr<SupportFunctionContent<Number>> pThis;

	SupportFunctionContent( const matrix_t<Number>& _shapeMatrix, SF_TYPE _type = SF_TYPE::ELLIPSOID );
	SupportFunctionContent( Number _radius, unsigned dimension, SF_TYPE _type = SF_TYPE::INFTY_BALL );
	SupportFunctionContent( const matrix_t<Number>& _directions, const vector_t<Number>& _distances,
					 SF_TYPE _type = SF_TYPE::POLY );
	SupportFunctionContent( const std::vector<Halfspace<Number>>& _planes, SF_TYPE _type = SF_TYPE::POLY );
	SupportFunctionContent( const std::vector<Point<Number>>& _points, SF_TYPE _type = SF_TYPE::POLY );
	SupportFunctionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _lhs, const std::shared_ptr<SupportFunctionContent<Number>>& _rhs,
					 SF_TYPE _type );
	SupportFunctionContent( const std::vector<std::shared_ptr<SupportFunctionContent<Number>>>& rhs, SF_TYPE type = SF_TYPE::UNITE );
	SupportFunctionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, const matrix_t<Number>& A, const vector_t<Number>& b, SF_TYPE _type );
	SupportFunctionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, const Number& _factor,
					 SF_TYPE _type = SF_TYPE::SCALE );
	SupportFunctionContent( const std::shared_ptr<SupportFunctionContent<Number>>& _origin, const std::vector<unsigned>& dimensions, SF_TYPE _type = SF_TYPE::PROJECTION );
	SupportFunctionContent( const SupportFunctionContent<Number>& _orig );

  public:

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE _type, matrix_t<Number> _shapeMatrix ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( _shapeMatrix, _type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE _type, Number _radius, unsigned dimension ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( _radius, dimension, _type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE _type, const matrix_t<Number>& _directions,
																	const vector_t<Number>& _distances ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( _directions, _distances, _type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE _type, const std::vector<Halfspace<Number>>& _planes ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( _planes, _type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE _type, const std::vector<Point<Number>>& _points ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( _points, _type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE type, const std::shared_ptr<SupportFunctionContent<Number>>& lhs, const std::shared_ptr<SupportFunctionContent<Number>>& rhs ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>( lhs, rhs, type ));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( SF_TYPE type, const std::vector<std::shared_ptr<SupportFunctionContent<Number>>>& rhs ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>(rhs, type));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( const std::shared_ptr<SupportFunctionContent<Number>>& orig, const matrix_t<Number>& constraints,
															const vector_t<Number>& constants ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>(orig, constraints, constants, SF_TYPE::LINTRAFO));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( const std::shared_ptr<SupportFunctionContent<Number>>& orig, Number factor ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>(orig, factor, SF_TYPE::SCALE));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( const std::shared_ptr<SupportFunctionContent<Number>>& orig, const std::vector<unsigned>& dimensions ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>(orig, dimensions, SF_TYPE::PROJECTION));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	static std::shared_ptr<SupportFunctionContent<Number>> create( const std::shared_ptr<SupportFunctionContent<Number>>& orig ) {
		auto obj = std::shared_ptr<SupportFunctionContent<Number>>( new SupportFunctionContent<Number>(orig));
		obj->pThis = obj;
		assert(obj->checkTreeValidity());
		return obj;
	}

	virtual ~SupportFunctionContent();

	std::shared_ptr<SupportFunctionContent<Number>>& operator=( const std::shared_ptr<SupportFunctionContent<Number>>& _orig ) ;

	EvaluationResult<Number> evaluate( const vector_t<Number>& _direction, bool useExact ) const;
	std::vector<EvaluationResult<Number>> multiEvaluate( const matrix_t<Number>& _directions, bool useExact ) const;

	std::size_t dimension() const;
	SF_TYPE type() const;
	unsigned depth() const;
	unsigned operationCount() const;
	std::shared_ptr<SupportFunctionContent<Number>> getThis() const { return std::shared_ptr<SupportFunctionContent<Number>>(pThis); }

	/**
	 * Returns an approximation of the number of mv multiplications neccessary for an evaluation of the SF
	 */
	unsigned multiplicationsPerEvaluation() const;

	void forceLinTransReduction();

	Point<Number> supremumPoint() const;

	std::vector<unsigned> collectProjections() const;

	// getter for the union types
	sumContent<Number>* summands() const;
	scaleContent<Number>* scaleParameters() const;
	trafoContent<Number>* linearTrafoParameters() const;
	unionContent<Number>* unionParameters() const;
	intersectionContent<Number>* intersectionParameters() const;
	projectionContent<Number>* projectionParameters() const;
	PolytopeSupportFunction<Number>* polytope() const;
	BallSupportFunction<Number>* ball() const;
	EllipsoidSupportFunction<Number>* ellipsoid() const;

	std::shared_ptr<SupportFunctionContent<Number>> project(const std::vector<unsigned>& dimensions) const;
	std::shared_ptr<SupportFunctionContent<Number>> affineTransformation( const matrix_t<Number>& A, const vector_t<Number>& b ) const;
	std::shared_ptr<SupportFunctionContent<Number>> minkowskiSum( const std::shared_ptr<SupportFunctionContent<Number>>& _rhs ) const;
	std::shared_ptr<SupportFunctionContent<Number>> intersect( const std::shared_ptr<SupportFunctionContent<Number>>& _rhs ) const;
	bool contains( const Point<Number>& _point ) const;
	bool contains( const vector_t<Number>& _point ) const;
	std::shared_ptr<SupportFunctionContent<Number>> unite( const std::shared_ptr<SupportFunctionContent<Number>>& _rhs ) const;
	static std::shared_ptr<SupportFunctionContent<Number>> unite( const std::vector<std::shared_ptr<SupportFunctionContent<Number>>>& _rhs );
	std::shared_ptr<SupportFunctionContent<Number>> scale( const Number& _factor = 1 ) const;

	bool empty() const;

	void print() const;
	friend std::ostream& operator<<( std::ostream& lhs, const std::shared_ptr<SupportFunctionContent<Number>>& rhs ) {
		unsigned level = 0;
		//std::cout << "Depth: " << rhs->mDepth << std::endl;
		while(true){
			std::string tmp = rhs->printLevel(level, "   ");
			if(!tmp.empty()) {
				lhs << rhs->printLevel(level, "   ") << std::endl;
				++level;
			} else {
				break;
			}
		}
		return lhs;
	}

	bool checkTreeValidity() const {
		assert(!pThis.expired());
		assert(this == pThis.lock().get());
		switch ( mType ) {
			case SF_TYPE::TWO_BALL:
			case SF_TYPE::INFTY_BALL:
			case SF_TYPE::POLY:
			case SF_TYPE::ELLIPSOID:
			case SF_TYPE::BOX:
			case SF_TYPE::ZONOTOPE:
				//std::cout << __func__ << ": Reached bottom, return true." << std::endl;
				return true;
			case SF_TYPE::LINTRAFO: {
				assert( linearTrafoParameters()->origin != nullptr);
				assert( linearTrafoParameters()->origin->type() != SF_TYPE::NONE );
				assert( linearTrafoParameters()->origin->checkTreeValidity());
				//return mLinearTrafoParameters->origin->checkTreeValidity();
				return true;
			}
			case SF_TYPE::SCALE: {
				//assert(scaleParameters()->origin->checkTreeValidity());
				assert(scaleParameters()->origin != nullptr);
				assert(scaleParameters()->origin->type() != SF_TYPE::NONE );
				return scaleParameters()->origin->checkTreeValidity();
				//return true;
			}
			case SF_TYPE::PROJECTION: {
				//assert(projectionParameters()->origin->checkTreeValidity());
				assert(projectionParameters()->origin != nullptr);
				assert(projectionParameters()->origin->type() != SF_TYPE::NONE);
				return projectionParameters()->origin->checkTreeValidity();
				//return true;
			}
			case SF_TYPE::SUM: {
				//assert(summands()->lhs->checkTreeValidity());
				//assert(summands()->rhs->checkTreeValidity());
				assert(summands()->lhs != nullptr);
				assert(summands()->rhs != nullptr);
				assert(summands()->lhs->type() != SF_TYPE::NONE);
				assert(summands()->rhs->type() != SF_TYPE::NONE);
				return summands()->lhs->checkTreeValidity() && summands()->rhs->checkTreeValidity();
				//return true;
			}
			case SF_TYPE::UNITE: {
				assert(!unionParameters()->items.empty());
				for(const auto& item : unionParameters()->items) {
					//assert(item->checkTreeValidity());

					if(!item->checkTreeValidity()){
						std::cerr << __func__ << ": One UNION Content was not valid!" << std::endl << std::flush;
						assert(false);
						return false;
					}
				}
				return true;
			}
			case SF_TYPE::INTERSECT: {
				//assert(mIntersectionParameters->lhs->checkTreeValidity());
				//assert(mIntersectionParameters->rhs->checkTreeValidity());
				assert(intersectionParameters()->lhs != nullptr);
				assert(intersectionParameters()->rhs != nullptr);
				assert(intersectionParameters()->lhs->type() != SF_TYPE::NONE);
				assert(intersectionParameters()->rhs->type() != SF_TYPE::NONE);
				return intersectionParameters()->lhs->checkTreeValidity() && intersectionParameters()->rhs->checkTreeValidity();
				return true;
			}
			default:
				std::cerr << __func__ << ": Default case, SHOULD NOT HAPPEN!" << std::endl << std::flush;
				assert(false);
				return false;
		}
	}

	private:

	bool hasTrafo(std::shared_ptr<const lintrafoParameters<Number>>& resNode, const matrix_t<Number>& A, const vector_t<Number>& b) const {
		switch ( mType ) {
			case SF_TYPE::SUM: {
				bool res = mSummands->lhs->hasTrafo(resNode, A, b);
				if(!res) {
					res = mSummands->rhs->hasTrafo(resNode, A, b);
				}
				return res;
			}
			case SF_TYPE::INTERSECT: {
				bool res = mIntersectionParameters->lhs->hasTrafo(resNode, A, b);
				if(!res) {
					res = mIntersectionParameters->rhs->hasTrafo(resNode, A, b);
				}
				return res;
			}
			case SF_TYPE::LINTRAFO: {
				if(mLinearTrafoParameters->parameters->matrix() == A && mLinearTrafoParameters->parameters->vector() == b) {
					resNode = mLinearTrafoParameters->parameters;
					return true;
				}
				return mLinearTrafoParameters->origin->hasTrafo(resNode, A, b);
			}
			case SF_TYPE::SCALE: {
				return mScaleParameters->origin->hasTrafo(resNode, A, b);
			}
			case SF_TYPE::UNITE: {
				for(const auto& item : mUnionParameters->items) {
					if(item->hasTrafo(resNode,A,b)) {
						return true;
					}
				}
				return false;
			}
			case SF_TYPE::POLY:
			case SF_TYPE::INFTY_BALL:
			case SF_TYPE::TWO_BALL:
			case SF_TYPE::ELLIPSOID:
			case SF_TYPE::BOX:
			case SF_TYPE::ZONOTOPE: {
				return false;
			}
			case SF_TYPE::PROJECTION: {
				return mProjectionParameters->origin->hasTrafo(resNode, A, b);
				break;
			}
			case SF_TYPE::NONE:
			default:
				assert(false);
				return false;
		}
	}

	std::vector<SF_TYPE> collectLevelEntries(unsigned level) const {
		//std::cout << __func__ << ": level: " << level << std::endl;
		std::vector<SF_TYPE> items;
		if(level == 0) {
			items.push_back(this->mType);
			//std::cout << "items push " << this->mType << std::endl;
			//std::cout << "items size: " << items.size() << std::endl;
			return items;
		}

		// Note that at this point we can omit terminal cases, as the level is larger than the depht of this subtree.
		switch ( mType ) {
			case SF_TYPE::LINTRAFO: {
				//std::cout << "Current: lintrafor" << std::endl;
				std::vector<SF_TYPE> tmpItems = mLinearTrafoParameters->origin->collectLevelEntries(level-1);
				items.insert(items.end(), tmpItems.begin(), tmpItems.end());
				return items;
			}
			case SF_TYPE::SCALE: {
				//std::cout << "Current: scale" << std::endl;
				std::vector<SF_TYPE> tmpItems = mScaleParameters->origin->collectLevelEntries(level-1);
				items.insert(items.end(), tmpItems.begin(), tmpItems.end());
				return items;
			}
			case SF_TYPE::PROJECTION: {
				//std::cout << "Current: projection" << std::endl;
				std::vector<SF_TYPE> tmpItems = mProjectionParameters->origin->collectLevelEntries(level-1);
				items.insert(items.end(), tmpItems.begin(), tmpItems.end());
				return items;
			}
			case SF_TYPE::SUM: {
				//std::cout << "Current: sum" << std::endl;
				std::vector<SF_TYPE> lhsItems = mSummands->lhs->collectLevelEntries(level-1);
				std::vector<SF_TYPE> rhsItems = mSummands->rhs->collectLevelEntries(level-1);
				items.insert(items.end(), lhsItems.begin(), lhsItems.end());
				items.insert(items.end(), rhsItems.begin(), rhsItems.end());
				return items;
			}
			case SF_TYPE::UNITE: {
				//std::cout << "Current: union" << std::endl;
				std::vector<SF_TYPE> res;
				for(const auto& item : mUnionParameters->items) {
					std::vector<SF_TYPE> tmp = item->collectLevelEntries(level-1);
					res.insert(res.end(), tmp.begin(), tmp.end());
				}
				return res;
			}
			case SF_TYPE::INTERSECT: {
				//std::cout << "Current: intersect" << std::endl;
				std::vector<SF_TYPE> lhsItems = mIntersectionParameters->lhs->collectLevelEntries(level-1);
				std::vector<SF_TYPE> rhsItems = mIntersectionParameters->rhs->collectLevelEntries(level-1);
				items.insert(items.end(), lhsItems.begin(), lhsItems.end());
				items.insert(items.end(), rhsItems.begin(), rhsItems.end());
				return items;
			}
			default:
				return std::vector<SF_TYPE>();
		}
	}

	std::string printLevel(unsigned l, std::string separator = "\t") const {
		//std::cout << "Print level" << std::endl;
		std::string level;
		std::vector<SF_TYPE> items = this->collectLevelEntries(l);
		if(items.empty())
			return level;

		//std::cout << "With " << items.size() << " entries." << std::endl;
		std::string unaryLevelTransition 	= "  |  "; // 1
		std::string binaryLevelTransition 	= "  |  " + separator + "\\    "; // 2
		std::string emptyLevelTransition 	= "     "; // 0
		std::vector<unsigned> transitionType;

		for(unsigned index = 0; index < items.size(); ++index) {
			switch ( items.at(index) ) {
				case SF_TYPE::BOX: {
					level += "BOX  " + separator;
					transitionType.push_back(0);
					break;
				}
				case SF_TYPE::ELLIPSOID: {
					level += "ELLIP" + separator;
					transitionType.push_back(0);
					break;
				}
				case SF_TYPE::INFTY_BALL: {
					level += "IBALL" + separator;
					transitionType.push_back(0);
					break;
				}
				case SF_TYPE::TWO_BALL: {
					level += "2BALL" + separator;
					transitionType.push_back(0);
					break;
				}
				case SF_TYPE::LINTRAFO: {
					level += "LINTR" + separator;
					transitionType.push_back(1);
					break;
				}
				case SF_TYPE::POLY: {
					level += "POLY " + separator;
					transitionType.push_back(0);
					break;
				}
				case SF_TYPE::PROJECTION: {
					level += "PROJ " + separator;
					transitionType.push_back(1);
					break;
				}
				case SF_TYPE::SCALE: {
					level += "SCALE" + separator;
					transitionType.push_back(1);
					break;
				}
				case SF_TYPE::SUM: {
					level += "SUM   " + separator + "     " + separator;
					transitionType.push_back(2);
					break;
				}
				case SF_TYPE::UNITE: {
					level += "UNITE" + separator + "     " + separator;
					transitionType.push_back(2);
					break;
				}
				case SF_TYPE::INTERSECT: {
					level += "INTSC" + separator + "     " + separator;
					transitionType.push_back(2);
					break;
				}
				case SF_TYPE::ZONOTOPE: {
					level += "ZONO " + separator;
					transitionType.push_back(0);
					break;
				}
				default:
					level += "NONE " + separator;
					transitionType.push_back(0);
			}
		}

		level += "\n";

		for(unsigned index = 0; index < transitionType.size(); ++index){
			switch (transitionType.at(index)){
				case 0: {
					level += emptyLevelTransition + separator;
					break;
				}
				case 1: {
					level += unaryLevelTransition + separator;
					break;
				}
				case 2: {
					level += binaryLevelTransition + separator;
					break;
				}
				default:
					assert(false);
			}
		}

		return level;
	}
};
}  // namespace

#include "SupportFunctionContent.tpp"
