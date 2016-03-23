#include "HPolytope.h"
namespace hypro {
template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT()
	: mHPlanes(), mFanSet( false ), mFan(), mDimension( 0 ), mEmpty(State::NSET), mNonRedundant(true) {
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const HPolytopeT<Number,Converter>& orig )
	: mHPlanes(orig.mHPlanes), mFanSet( orig.mFanSet ), mFan( orig.mFan ), mDimension( orig.mDimension ), mEmpty(orig.mEmpty), mNonRedundant(orig.mNonRedundant) {
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const HyperplaneVector &planes )
	: mHPlanes(), mFanSet( false ), mFan(), mDimension( 0 ), mEmpty(State::NSET), mNonRedundant(false) {
	if ( !planes.empty() ) {
		mDimension = planes.begin()->dimension();
		for ( const auto &plane : planes ) {
			mHPlanes.push_back( plane );
		}
		reduceNumberRepresentation();
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const matrix_t<Number> &A, const vector_t<Number> &b )
	: mHPlanes(), mFanSet( false ), mFan(), mDimension( A.cols() ), mEmpty(State::NSET), mNonRedundant(false) {
	assert( A.rows() == b.rows() );
	for ( unsigned i = 0; i < A.rows(); ++i ) {
		mHPlanes.push_back( Hyperplane<Number>( A.row( i ), b( i ) ) );
	}
	reduceNumberRepresentation();
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const matrix_t<Number> &A )
	: mHPlanes(), mFanSet( false ), mFan(), mDimension( A.cols() ), mEmpty(State::NSET), mNonRedundant(false) {
	for ( unsigned i = 0; i < A.rows(); ++i ) {
		mHPlanes.push_back( Hyperplane<Number>( A.row( i ), Number( 0 ) ) );
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const std::vector<Point<Number>>& points )
	: mHPlanes(), mFanSet( false ), mFan(), mDimension( 0 ), mEmpty(State::NSET), mNonRedundant(false) {
	if ( !points.empty() ) {
		// degenerate cases
		unsigned size = points.size();
		mDimension = points.begin()->dimension();
		if ( size == 1 ) {
			// Return Box constraints.
		} else if ( size < mDimension ) {
			// ATTENTION: Assumption here: alien is reduced, such that the d points in alien span a d-1 dimensional object.
			// find all hyperplanar descriptions by reducing to d dimensions (get the plane)
			std::size_t size = points.size();
			Permutator permutator(mDimension, size);
			std::vector<unsigned> permutation;
			while(!permutator.end()) {
				permutation = permutator();
				// project to chosen dimensions
				std::vector<Point<Number>> reducedVertices;
				reducedVertices.reserve(size);
				for(const auto& vertex : points) {
					vector_t<Number> reductor = vector_t<Number>(size);
					for(unsigned d = 0; d < size; ++d)
						reductor(d) = vertex.at(d);
					reducedVertices.push_back(Point<Number>(std::move(reductor)));
				}
				std::vector<std::shared_ptr<Facet<Number>>> facets = convexHull( reducedVertices ).first;
				//std::cout << "Conv Hull end" << std::endl;
				for ( auto &facet : facets ) {
					mHPlanes.push_back( facet->hyperplane() );
				}
			}
			assert( false );
		} else {
			//std::cout << "Conv Hull" << std::endl;
			// TODO: Chose suitable convex hull algorithm
			std::vector<std::shared_ptr<Facet<Number>>> facets = convexHull( points ).first;
			//std::cout << "Conv Hull end" << std::endl;
			for ( auto &facet : facets ) {
				mHPlanes.push_back( facet->hyperplane() );
			}
			facets.clear();
		}
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::~HPolytopeT() {
}

/*
 * Getters and setters
 */

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::empty() const {
	std::cout << __func__ << ": State: " << mEmpty << std::endl;
	if(mEmpty == State::TRUE)
		return true;
	if(mEmpty == State::FALSE)
		return false;

	if(mHPlanes.empty()){
		mEmpty = State::FALSE;
		return false;
	}

	std::cout << __func__ << ": CALL TO Optimizer" << std::endl;

	Optimizer<Number>& opt = Optimizer<Number>::getInstance();
	opt.setMatrix(this->matrix());
	opt.setVector(this->vector());

	bool res = !opt.checkConsistency();
	mEmpty = (res == true ? State::TRUE : State::FALSE);
	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::Empty(){
	Hyperplane<Number> a({1},-1);
	Hyperplane<Number> b({-1},-1);
	HyperplaneVector v;
	v.emplace_back(a);
	v.emplace_back(b);
	HPolytopeT<Number, Converter> res(v);
	res.mEmpty=State::TRUE;
	res.mNonRedundant=true;
	return res;
}

template <typename Number, typename Converter>
std::size_t HPolytopeT<Number, Converter>::dimension() const {
	if(mHPlanes.empty()) return 0;
	return mDimension;
}

template <typename Number, typename Converter>
std::size_t HPolytopeT<Number, Converter>::size() const {
	return mHPlanes.size();
}

template <typename Number, typename Converter>
matrix_t<Number> HPolytopeT<Number, Converter>::matrix() const {
	matrix_t<Number> res( mHPlanes.size(), dimension() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		res.row( planeIndex ) = mHPlanes.at( planeIndex ).normal();
	}
	return res;
}

template <typename Number, typename Converter>
vector_t<Number> HPolytopeT<Number, Converter>::vector() const {
	vector_t<Number> res( mHPlanes.size() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		res( planeIndex ) = mHPlanes.at( planeIndex ).offset();
	}
	return res;
}

template <typename Number, typename Converter>
std::pair<matrix_t<Number>, vector_t<Number>> HPolytopeT<Number, Converter>::inequalities() const {
	matrix_t<Number> A( mHPlanes.size(), dimension() );
	vector_t<Number> b( mHPlanes.size() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		A.row( planeIndex ) = mHPlanes.at( planeIndex ).normal().transpose();
		b( planeIndex ) = mHPlanes.at( planeIndex ).offset();
	}
	return std::make_pair( A, b );
}

template <typename Number, typename Converter>
const typename polytope::Fan<Number> &HPolytopeT<Number, Converter>::fan() const {
	if ( !mFanSet ) {
		calculateFan();
	}
	return mFan;
}

template <typename Number, typename Converter>
typename std::vector<Point<Number>> HPolytopeT<Number, Converter>::vertices() const {
	//std::cout << "Compute vertices of " << *this << std::endl;
	typename std::vector<Point<Number>> vertices;
	if(!mHPlanes.empty()) {
		unsigned dim = this->dimension();

		Permutator permutator(mHPlanes.size(), dim);
		std::vector<unsigned> permutation;
		while(!permutator.end()) {
			permutation = permutator();
			//std::cout << "Use planes ";
			//for(const auto item : permutation)
			//	std::cout << item << ", ";
			//std::cout << std::endl;

			matrix_t<Number> A( dim, dim );
			vector_t<Number> b( dim );
			unsigned pos = 0;
			for(auto planeIt = permutation.begin(); planeIt != permutation.end(); ++planeIt) {
				A.row(pos) = mHPlanes.at(*planeIt).normal().transpose();
				// std::cout << A.row(pos) << std::endl;
				b(pos) = mHPlanes.at(*planeIt).offset();
				// std::cout << b(pos) << std::endl;
				++pos;
			}

			//std::cout << "Created first matrix" << std::endl;

			Eigen::FullPivLU<matrix_t<Number>> lu_decomp( A );
			if ( lu_decomp.rank() < A.rows() ) {
				continue;
			}

			vector_t<Number> res = lu_decomp.solve( b );

			// check for infinity
			bool infty = false;
			//for ( unsigned i = 0; i < res.rows(); ++i ) {
			//	if ( std::numeric_limits<Number>::infinity() == ( Number( res( i ) ) ) ) {
			//		std::cout << ( Number( res( i ) ) ) << " is infty." << std::endl;
			//		infty = true;
			//		break;
			//	}
			//}

			if(!infty) {
				//std::cout << "Solved to " << res.transpose() << std::endl;

                ///// if it's not almost equal, then
				// Check if the computed vertex is a real vertex
				bool outside = false;
				for(unsigned planePos = 0; planePos < mHPlanes.size(); ++planePos) {
					bool skip = false;
					for(unsigned permPos = 0; permPos < permutation.size(); ++permPos) {
						if(planePos == permutation.at(permPos)) {
							//std::cout << "Skip plane " << planePos << std::endl;
							skip = true;
							break;
						}
					}

					if(!skip) {
						if( mHPlanes.at(planePos).offset() - mHPlanes.at(planePos).normal().dot(res) < 0 ) {
							//std::cout << "Drop vertex: " << res << " because of plane " << planePos << std::endl;
							outside = true;
							break;
						}
					}
				}
				if(!outside) {
					vertices.emplace_back(res);
					//std::cout << "Final vertex: " << res << std::endl;
				}
			}
		}
	}
	return vertices;
}

template <typename Number, typename Converter>
Number HPolytopeT<Number, Converter>::supremum() const {
	Number max = 0;
	for ( auto &point : this->vertices() ) {
		Number inftyNorm = hypro::Point<Number>::inftyNorm( point );
		max = max > inftyNorm ? max : inftyNorm;
	}
	return max;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::calculateFan() const {
	std::vector<std::shared_ptr<Facet<Number>>> facets = convexHull( vertices() ).first;
	std::set<Point<Number>> preresult;
	for ( unsigned i = 0; i < facets.size(); i++ ) {
		for ( unsigned j = 0; j < facets[i].vertices().size(); j++ ) {
			preresult.insert( facets[i]->vertices().at( j ) );
		}
	}
	polytope::Fan<Number> fan;
	for ( auto &point : preresult ) {
		polytope::Cone<Number> *cone = new polytope::Cone<Number>();
		for ( unsigned i = 0; i < facets.size(); i++ ) {
			for ( unsigned j = 0; j < facets[i]->vertices().size(); j++ ) {
				if ( point == facets[i]->vertices().at( j ) ) {
					std::vector<Ridge<Number>> ridges = getRidges( *facets[i] );
					for ( unsigned m = 0; m < ridges.size(); m++ ) {
						if ( checkInsideRidge( ridges[m], point ) ) {
							std::vector<Facet<Number>> conefacets = shareRidge( facets, ridges[m] );

							matrix_t<Number> matrix = matrix_t<Number>( conefacets.size(), point.size() );
							for ( unsigned k = 1; k < conefacets.size(); k++ ) {
								for ( unsigned l = 0; l < conefacets[k].getNormal().size(); l++ ) {
									matrix( k, l ) = conefacets[k].getNormal()( l );
								}
							}

							for ( unsigned j = 0; j < point.size(); j++ ) {
								matrix( 0, j ) = 1;

								if ( matrix.fullPivLu().rank() == point.size() ) {
									break;
								} else {
									matrix( 0, j ) = 0;
								}
							}
							vector_t<Number> b = vector_t<Number>::Zero( conefacets.size() );
							b( 0 ) = 1;
							vector_t<Number> result = matrix.fullPivHouseholderQr().solve( b );

							cone->add( std::shared_ptr<Hyperplane<Number>>(
								  new Hyperplane<Number>( result, result.dot( point.rawCoordinates() ) ) ) );
							// cone->add(std::make_shared<Hyperplane<Number>>(Hyperplane<Number>(result,
							// result.dot(point.rawCoordinates()))));
						}
					}
				}
			}
		}
		fan.add( cone );
	}
	mFanSet = true;
	mFan = fan;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::insert( const Hyperplane<Number> &plane ) {
	std::cout << "Dimension: " << mDimension << ", plane dimension: " << plane.dimension() << std::endl;
	assert( mDimension == 0 || mDimension == plane.dimension() );
	if ( mDimension == 0 ) {
		mDimension = plane.dimension();
	}
	bool found = false;
	for(auto planeIt = mHPlanes.begin(); planeIt != mHPlanes.end(); ++planeIt) {
		if(*planeIt == plane){
			found = true;
			break;
		}
	}
	if(!found){
		mHPlanes.push_back( plane );
		mEmpty = State::NSET;
		mNonRedundant = false;
	}
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::insert( const typename HyperplaneVector::iterator begin,
								const typename HyperplaneVector::iterator end ) {
	assert( mDimension == 0 || mDimension == begin->dimension() );
	if ( mDimension == 0 ) {
		mDimension = begin->dimension();
	}
	auto it = begin;
	while ( it != end ) {
		this->insert( *it );
		++it;
	}
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::erase( const unsigned index ) {
	assert(index < mHPlanes.size());
	mHPlanes.erase(mHPlanes.begin()+index);
	mEmpty = State::NSET;
}

template <typename Number, typename Converter>
const typename HPolytopeT<Number, Converter>::HyperplaneVector &HPolytopeT<Number, Converter>::constraints() const {
	return mHPlanes;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::hasConstraint( const Hyperplane<Number> &hplane ) const {
	for ( const auto &plane : mHPlanes ) {
		if ( hplane == plane ) return true;
	}
	return false;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::removeRedundancy() {
	if(!mNonRedundant && mHPlanes.size() > 1){
		Optimizer<Number>& opt = Optimizer<Number>::getInstance();
		opt.setMatrix(this->matrix());
		opt.setVector(this->vector());

		std::vector<std::size_t> redundant = Optimizer<Number>::getInstance().redundantConstraints();

		if(!redundant.empty()){
			std::size_t cnt = mHPlanes.size()-1;
			for ( auto rIt = mHPlanes.rbegin(); rIt != mHPlanes.rend(); ++rIt ) {
				if(redundant.empty())
					break;

				if(redundant.back() == cnt){
					mHPlanes.erase( --(rIt.base()) );
					redundant.pop_back();
					//std::cout << "Erase plane " << cnt << std::endl;
				}
				--cnt;
			}
		}

		assert(redundant.empty());
	}
	mNonRedundant=true;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::isExtremePoint( const vector_t<Number>& point ) const {
	unsigned cnt = 0;
	for ( const auto &plane : mHPlanes ) {
		Number val = plane.evaluate( point );
		if ( plane.offset() == val  ) {
			++cnt;
		} else if ( plane.offset() - val < 0 ) {
			return false;
		}
	}
	return cnt >= mDimension;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::isExtremePoint( const Point<Number> &point ) const {
	return isExtremePoint( point.rawCoordinates() );
}

template <typename Number, typename Converter>
EvaluationResult<Number> HPolytopeT<Number, Converter>::evaluate( const vector_t<Number> &_direction ) const {
	if(mHPlanes.empty())
		return EvaluationResult<Number>( 1, INFTY );

	//reduceNumberRepresentation();

	std::cout << "Constraints: " << convert<Number,double>(this->matrix()) << std::endl << "Constants: " << this->vector() << std::endl;

	Optimizer<Number>& opt = Optimizer<Number>::getInstance();
	opt.setMatrix(this->matrix());
	opt.setVector(this->vector());

	return opt.evaluate(_direction);
}

/*
 * General interface
 */

template<typename Number, typename Converter>
std::pair<bool, HPolytopeT<Number, Converter>> HPolytopeT<Number, Converter>::satisfiesHyperplane( const vector_t<Number>& normal, const Number& offset ) const {
	HPolytopeT<Number,Converter> tmp = this->intersectHyperplane(Hyperplane<Number>(normal, offset));
	return std::make_pair(!(tmp).empty(), tmp);
}

template<typename Number, typename Converter>
std::pair<bool, HPolytopeT<Number, Converter>> HPolytopeT<Number, Converter>::satisfiesHyperplanes( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const {
	HPolytopeT<Number,Converter> tmp = this->intersectHyperplanes(_mat, _vec);
	return std::make_pair(!(tmp).empty(),tmp);
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::linearTransformation( const matrix_t<Number> &A,
														   const vector_t<Number> &b ) const {
	if(!this->empty() && !mHPlanes.empty()) {
		Eigen::FullPivLU<matrix_t<Number>> lu(A);
		// if A has full rank, we can simply re-transform, otherwise use v-representation.
		if(lu.rank() == A.rows()) {
			//std::cout << "Full rank, retransform!" << std::endl;
			std::pair<matrix_t<Number>, vector_t<Number>> inequalities = this->inequalities();
			//std::cout << "Matrix: " << inequalities.first*A.inverse() << std::endl << "Vector: " << ((inequalities.first*A.inverse()*b) + (inequalities.second)) << std::endl;
			return HPolytopeT<Number, Converter>(inequalities.first*A.inverse(), inequalities.first*A.inverse()*b + inequalities.second);
		} else {
			//std::cout << "Use V-Conversion for linear transformation." << std::endl;
			auto intermediate = Converter::toVPolytope( *this );
			intermediate = intermediate.linearTransformation( A, b );
			auto res = Converter::toHPolytope(intermediate);
			return res;
		}
	} else {
		return *this;
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::minkowskiSum( const HPolytopeT &rhs ) const {
	HPolytopeT<Number, Converter> res;
	Number result;

	std::cout << __func__ << " of " << *this << " and " << rhs << std::endl;

	// evaluation of rhs in directions of lhs
	std::cout << "evaluation of rhs in directions of lhs" << std::endl;
	for ( unsigned i = 0; i < mHPlanes.size(); ++i ) {
		EvaluationResult<Number> evalRes = rhs.evaluate( mHPlanes.at( i ).normal() );
		if ( evalRes.errorCode == INFTY ) {
			std::cout << __func__ << " Evaluated against " <<
			mHPlanes.at(i).normal() << std::endl;
			std::cout << "INFTY" << std::endl;
			// Do nothing - omit inserting plane.
		} else if ( evalRes.errorCode == INFEAS ) {
			std::cout << "EMPTY" << std::endl;
			return Empty();
		} else {
			result = mHPlanes.at( i ).offset() + evalRes.supportValue;
			res.insert( Hyperplane<Number>( mHPlanes.at( i ).normal(), result ) );
			std::cout << __func__ << " Evaluated against " <<
			mHPlanes.at(i).normal() << " results in a distance " << evalRes.supportValue << std::endl;
			std::cout << "Old distance: " << carl::toDouble(mHPlanes.at(i).offset()) << ", new distance: " << carl::toDouble(result) << std::endl;
		}
	}

	// evaluation of lhs in directions of rhs
	std::cout << "evaluation of lhs in directions of rhs" << std::endl;
	for ( unsigned i = 0; i < rhs.constraints().size(); ++i ) {
		EvaluationResult<Number> evalRes = this->evaluate( rhs.constraints().at( i ).normal() );
		if ( evalRes.errorCode == INFTY ) {
			std::cout << __func__ << " Evaluated against " <<
			rhs.constraints().at( i ).normal() << std::endl;
			std::cout << "INFTY" << std::endl;
			// Do nothing - omit inserting plane.
		} else if ( evalRes.errorCode == INFEAS ) {
			std::cout << "EMPTY" << std::endl;
			return Empty();
		} else {
			result = rhs.constraints().at( i ).offset() + evalRes.supportValue;
			res.insert( Hyperplane<Number>( rhs.constraints().at( i ).normal(), result ) );
			std::cout << __func__ << " Evaluated against " <<
			rhs.constraints().at( i ).normal() << " results in a distance " << evalRes.supportValue << std::endl;
		}
	}
	std::cout << "Result: " << res << std::endl;
	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersect( const HPolytopeT &rhs ) const {
	if ( rhs.empty() || this->empty() ) {
		return HPolytopeT<Number, Converter>::Empty();
	} else {
		HPolytopeT<Number, Converter> res;
		for ( const auto &plane : mHPlanes ) {
			res.insert( plane );
		}
		for ( const auto &plane : rhs.constraints() ) {
			res.insert( plane );
		}

		return res;
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersectHyperplane( const Hyperplane<Number> &rhs ) const {
	HPolytopeT<Number, Converter> res( *this );
	// only insert the new hyperplane, if it is not already redundant.
	if(res.evaluate(rhs.normal()) > rhs.offset())
		res.insert( Hyperplane<Number>( rhs ) );

	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersectHyperplanes( const matrix_t<Number> &_mat,
														   const vector_t<Number> &_vec ) const {
	assert( _mat.rows() == _vec.rows() );
	HPolytopeT<Number, Converter> res( *this );
	for ( unsigned i = 0; i < _mat.rows(); ++i ) {
		res.insert( Hyperplane<Number>( _mat.row( i ), _vec( i ) ) );
	}
	res.removeRedundancy();
	return res;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const Point<Number> &point ) const {
	return this->contains( point.rawCoordinates() );
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const vector_t<Number> &vec ) const {
	for ( const auto &plane : mHPlanes ) {
		if (plane.normal().dot( vec ) > plane.offset()) {
			return false;
		}
	}
	return true;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const HPolytopeT<Number, Converter> &rhs ) const {
	std::cout << __func__ << " : " << *this << " contains " << rhs << std::endl;
	if(this->empty()){
		std::cout << __func__ << ": THIS IS EMPTY" << std::endl;
		return false;
	}

	if(rhs.empty()) {
		return true;
	}

	for ( const auto &plane : rhs.constraints() ) {
		EvaluationResult<Number> evalRes = this->evaluate( plane.normal() );

		std::cout << __func__ << ": plane " << plane << " -> " << evalRes.supportValue  << " orig offset: " << plane.offset() << "\t" ;
		if ( evalRes.errorCode == INFEAS ) {
			std::cout << "INFEAS" << std::endl;
			assert(false);
			return false;  // empty!
		} else if ( evalRes.errorCode == INFTY ) {
			std::cout << "INFTY" << std::endl;
			continue;
		} else if ( evalRes.supportValue < plane.offset() ) {
			std::cout << "Too large" << std::endl;
			return false;
		}
	}
	return true;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::unite( const HPolytopeT &_rhs ) const {
	if ( _rhs.empty() && !this->empty()) {
		return *this;
	} else if (this->empty() && !_rhs.empty()) {
		return _rhs;
	} else { // none is empty
		auto lhs = Converter::toVPolytope( *this );
		auto tmpRes = lhs.unite( Converter::toVPolytope( _rhs ) );
		HPolytopeT<Number,Converter> result = Converter::toHPolytope( tmpRes );
		assert(result.contains(*this));
		assert(result.contains(_rhs));
		std::cout << __func__ << " : tmpres " << tmpRes << std::endl;

		return result;
	}
	// if both are empty, return one of them
	return *this;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::clear() {
	mHPlanes.clear();
	mFanSet = false;
	mDimension = 0;
	mEmpty = FALSE;
	mNonRedundant = true;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::print() const {
	std::cout << *this << std::endl;
}

/*
 * Operators
 */

//template <typename Number, typename Converter>
//const Hyperplane<Number>& HPolytopeT<Number, Converter>::operator[]( size_t i ) const {
//	return mHPlanes.at( i );
//}
//
//template <typename Number, typename Converter>
//Hyperplane<Number>& HPolytopeT<Number, Converter>::operator[]( size_t i ) {
//	return mHPlanes.at( i );
//}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> &HPolytopeT<Number, Converter>::operator=( const HPolytopeT<Number, Converter> &rhs ) {
	if ( this != &rhs ) {
		HPolytopeT<Number, Converter> tmp( rhs );
		swap( *this, tmp );
	}
	return *this;
}

/*
 * Auxiliary functions
 */

template<typename Number, typename Converter>
void HPolytopeT<Number, Converter>::reduceNumberRepresentation(unsigned limit) const {
	std::vector<Point<Number>> vertices = this->vertices();

	// normal reduction
	for(unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex){
		//std::cout << "Original: " << mHPlanes.at(planeIndex) << std::endl;
		// find maximal value
		Number largest = 0;
		mHPlanes.at(planeIndex).makeInteger();
		//std::cout << "As Integer: " << mHPlanes.at(planeIndex) << std::endl;
		largest = carl::abs(mHPlanes.at(planeIndex).offset());
		for(unsigned i = 0; i < mDimension; ++i){
			if(carl::abs(mHPlanes.at(planeIndex).normal()(i)) > largest){
				largest = carl::abs(mHPlanes.at(planeIndex).normal()(i));
			}
		}

		// reduce, if reduction is required
		if(largest > (limit*limit)) {
			vector_t<Number> newNormal(mDimension);
			for(unsigned i = 0; i < mDimension; ++i){
				newNormal(i) = carl::floor(Number((mHPlanes.at(planeIndex).normal()(i)/largest)*limit));
				assert(carl::abs(Number(mHPlanes.at(planeIndex).normal()(i)/largest)) <= 1);
				assert(carl::isInteger(newNormal(i)));
				assert(newNormal(i) <= limit);
			}
			mHPlanes.at(planeIndex).setNormal(newNormal);
			Number newOffset = mHPlanes.at(planeIndex).offset();
			newOffset = carl::ceil(Number((newOffset/largest)*limit));

			for(const auto& vertex : vertices) {
				Number tmp = newNormal.dot(vertex.rawCoordinates());
				if(tmp > newOffset){
					newOffset = newOffset + (tmp-newOffset);
					assert(newNormal.dot(vertex.rawCoordinates()) <= newOffset);
				}
			}
			newOffset = carl::ceil(newOffset);
			mHPlanes.at(planeIndex).setOffset(newOffset);
		}
		//std::cout << "Reduced: " << mHPlanes.at(planeIndex) << std::endl;
	}
}

}  // namespace hypro
