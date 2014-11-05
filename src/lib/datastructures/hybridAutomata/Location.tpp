#include "Location.h"

namespace hypro {

	template<typename Number>
	Location<Number>::Location(const Location& _loc) : mLocation(_loc.mLocation){
	}

	template<typename Number>
	Location<Number>::Location(const hypro::matrix_t<Number> _mat, const hypro::vector_t<Number> _vec, const transitionSet _trans, const struct invariant _inv){
		mLocation.vec = _vec;
		mLocation.mat = _mat;
		mLocation.inv = _inv;
		mLocation.trans = _trans;
	}

	template<typename Number>
	Location<Number>::Location(const hypro::matrix_t<Number> _mat, const hypro::vector_t<Number> _vec,
			const transitionSet _trans, const struct invariant _inv, const hypro::matrix_t<Number> _extInputMat){
		mLocation.vec = _vec;
		mLocation.mat = _mat;
		mLocation.inv = _inv;
		mLocation.trans = _trans;
		mLocation.extInputMat = _extInputMat;
	}

	template<typename Number>
	hypro::vector_t<Number> Location<Number>::activityVec() {
		return mLocation.vec;
	}

	template<typename Number>
	hypro::matrix_t<Number> Location<Number>::activityMat() {
		return mLocation.mat;
	}

	template<typename Number>
	hypro::matrix_t<Number> Location<Number>::extInputMat() {
		return mLocation.extInputMat;
	}

	template<typename Number>
	struct Location<Number>::invariant Location<Number>::invariant() {
		return mLocation.inv;
	}

	template<typename Number>
	struct Location<Number>::location Location<Number>::location() {
		return mLocation;
	}

	template<typename Number>
	std::set<Transition<Number>*> Location<Number>::transitions() {
		return mLocation.trans;
	}

	template<typename Number>
	void Location<Number>::setActivityVec(hypro::vector_t<Number> _vec) {
		mLocation.vec = _vec;
	}

	template<typename Number>
	void Location<Number>::setActivityMat(hypro::matrix_t<Number> _mat) {
		mLocation.mat = _mat;
	}

	template<typename Number>
	void Location<Number>::setExtInputMat(hypro::matrix_t<Number> _mat) {
		mLocation.extInputMat = _mat;
	}

	template<typename Number>
	void Location<Number>::setInvariant(struct hypro::Location<Number>::invariant _inv) {
		mLocation.inv = _inv;
	}

	template<typename Number>
	void Location<Number>::setInvariant(hypro::matrix_t<Number> _mat, hypro::vector_t<Number> _vec, hypro::operator_e _op) {
		mLocation.inv.op = _op;
		mLocation.inv.vec = _vec;
		mLocation.inv.mat = _mat;
	}

	template<typename Number>
	void Location<Number>::setLocation(struct location _loc) {
		mLocation = _loc;
	}

	template<typename Number>
	void Location<Number>::setTransitions(transitionSet _trans) {
		mLocation.trans = _trans;
	}

}
