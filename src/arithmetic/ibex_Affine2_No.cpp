/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_No> class without rounding
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_No.h"
#include "ibex_Affine2.h"

namespace ibex {


template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::operator=(const Interval& x) {

	if (x.is_empty()) {
		_n = -1;
		_elt._err = 0.0;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY && x.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = 0.0;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = x.lb();
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = x.ub();
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else  {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val	= new double[1];
		_elt._val[0] = x.mid();
		_elt._err	= x.rad();
	}
	return *this;
}



template<>
Affine2Main<AF_No>::Affine2Main() :
		 _n		(-2		),
		 _elt	(NULL	,POS_INFINITY)	{
 }

template<>
Affine2Main<AF_No>::Affine2Main(int n, int m, const Interval& itv) :
			_n 		(n),
			_elt	(NULL,0.0)
{
	assert((n>=0) && (m>=0) && (m<=n));
	if (!(itv.is_unbounded()||itv.is_empty())) {
		_elt._val	=new double[n + 1];
		_elt._val[0] = itv.mid();
		for (int i = 1; i <= n; i++){
			_elt._val[i] = 0.0;
		}

		if (m == 0) {
			_elt._err = itv.rad();
		} else {
			_elt._val[m] = itv.rad();
		}
	} else {
		*this = itv;
	}
}


template<>
Affine2Main<AF_No>::Affine2Main(const double d) :
			_n 		(0),
			_elt	(NULL, 0.0) {
	if (fabs(d)<POS_INFINITY) {
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		if (d>0) {
			_n = -3;
		} else {
			_n = -4;
		}
		_elt._err = d;
	}
}


template<>
Affine2Main<AF_No>::Affine2Main(const Interval & itv):
			_n 		(0),
			_elt	(NULL, 0.0) {

	if (itv.is_empty()) {
		_n = -1;
	} else if (itv.ub()>= POS_INFINITY && itv.lb()<= NEG_INFINITY ) {
		_n = -2;
	} else if (itv.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = itv.lb();
	} else if (itv.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = itv.ub();
	} else  {
		_n = 0;
		_elt._val	= new double[1];
		_elt._val[0] = itv.mid();
		_elt._err	= itv.rad();
	}
}


template<>
Affine2Main<AF_No>::Affine2Main(const Affine2Main<AF_No>& x) :
		_n		(x._n),
		_elt	(NULL	 ,x._elt._err ) {
	if (is_actif()) {
		_elt._val =new double[x.size() + 1];
		for (int i = 0; i <= x.size(); i++){
			_elt._val[i] = x._elt._val[i];
		}
	}
}




template<>
double Affine2Main<AF_No>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _elt._val[i];
}

template<>
double Affine2Main<AF_No>::err() const{
	return _elt._err;
}


template<>
const Interval Affine2Main<AF_No>::itv() const {

	if (is_actif()) {
		Interval res(_elt._val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_elt._val[i] * pmOne);
		}
		res += _elt._err * pmOne;
		return res;
	} else if (_n==-1) {
		return Interval::EMPTY_SET;
	} else if (_n==-2) {
		return Interval::ALL_REALS;
	} else if (_n==-3) {
		return Interval(_elt._err,POS_INFINITY);
	} else  {  //if (_n==-4)
		return Interval(NEG_INFINITY,_elt._err);
	}

}


template<>
double Affine2Main<AF_No>::mid() const{
	return (is_actif())? _elt._val[0] : itv().mid();
}


template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::operator=(const Affine2Main<AF_No>& x) {
	if (this != &x) {
		_elt._err = x._elt._err;
		if (x.is_actif()) {
			if (_n!=x._n) {
				_n =x._n;
				if (_elt._val!=NULL) { delete[] _elt._val; }
				_elt._val = new double[_n+1];
			}
			if (_elt._val==NULL) _elt._val = new double[_n+1];

			for (int i = 0; i <= x.size(); i++) {
				_elt._val[i] = x._elt._val[i];
			}
		} else {
			_n = x._n;
			if (_elt._val != NULL) {
				delete[] _elt._val;
				_elt._val = NULL;
			}
		}
	}
	return *this;

}

template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::operator=(double d) {

	if (fabs(d)<POS_INFINITY) {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		if (d>0) {
			_n = -3;
		} else {
			_n = -4;
		}
		_elt._err = d;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	}
	return *this;
}



/** \brief Return (-x) */
template<>
Affine2Main<AF_No> Affine2Main<AF_No>::operator-() const {
	Affine2Main<AF_No> res;
	res._n = _n;
	res._elt._err = _elt._err;
	if (is_actif()) {
		res._elt._val = new double[_n+1];
		for (int i = 0; i <= _n; i++) {
			res._elt._val[i] = (-_elt._val[i]);
		}

	}
	return res;
}




template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::saxpy(double alpha, const Affine2Main<AF_No>& y, double beta, double ddelta, bool B1, bool B2, bool B3, bool B4) {
//std::cout << "saxpy IN " << alpha << " x " << *this << " + " << y << " + "<< beta << " +error " << ddelta << " / "<< B1 << B2 << B3 << B4 << std::endl;

	int i;
//	std::cout << "in saxpy alpha=" << alpha  <<  "  beta= " <<  beta <<   "  delta = " << ddelta   << std::endl;
	if (is_actif()) {
		if (B1) {  // multiply by a scalar alpha
			if (alpha==0.0) {
				for (i=0; i<=_n;i++) {
					_elt._val[i]=0;
				}
				_elt._err = 0;
			}
			else if ((fabs(alpha)) < POS_INFINITY) {
				for (i=0; i<=_n;i++) {
					_elt._val[i] *= alpha;
				}

				_elt._err *= fabs(alpha);

			}
			else {
				*this = itv()*alpha;
			}
		}

		if (B2) {  // add a affine2 form y

			if (y.is_actif()) {
				if (_n==y.size()) {

					for(i=0;i<=_n;i++) {
						_elt._val[i] +=y._elt._val[i];
					}

					_elt._err += y._elt._err;

				} else  {
					if (_n>y.size()) {
						*this += y.itv();
					} else {
						Interval tmp1 = itv();
						*this = y;
						*this += tmp1;
					}
				}
			}
			else { // y is not a valid affine2 form. So we add y.itv() such as an interval
				*this = itv()+y.itv();
			}
		}
		if (B3) {  //add a constant beta
			if ((fabs(beta))<POS_INFINITY) {
				_elt._val[0] += beta;
			}
			else {
				*this = itv()+beta;
			}
		}

		if (B4) {  // add an error  ddelta

			if ((fabs(ddelta))<POS_INFINITY) {
				_elt._err += fabs(ddelta);

			}
			else {
				*this = itv()+Interval(-1,1)*ddelta;
			}
		}

		if (_elt._val != NULL) {
			bool b = (_elt._err<POS_INFINITY);
			for (i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	} else {
		if (B1) {  //scalar alpha
			*this = itv()* alpha;
		}
		if (B2) {  // add y
			*this = itv()+ y.itv();
		}
		if (B3) {  //constant beta
			*this = itv()+ beta;
		}
		if (B4) {  // error  delta
			*this = itv()+Interval(-1,1)*ddelta;
		}
	}
//	std::cout << " saxpy OUT x= "<< *this<<std::endl;
	return *this;

}


template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		double   yVal0;
		int i;
//std::cout << "in *  "<<y<<std::endl;
//saxpy(y.mid(), Affine2Main<AF_No>(), 0.0, y.rad(), true, false, false, true);

		yVal0 = y.mid();
		// RES = X%(0) * res
		for (i=0; i<=_n;i++) {
			_elt._val[i] *=yVal0;
		}

		//_elt._err *= (fabs(yVal0)+Interval(y.rad()));
		_elt._err *= (abs(y).ub());

		{
			bool b = (_elt._err<POS_INFINITY);
			for (i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	}
	return *this;
}


template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::operator*=(const Affine2Main<AF_No>& y) {
//	std::cout << "in *= "<< *this <<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx, Sy, Sxy, Sz, xVal0,tmp;
			int i;
			double * xTmp;

			xTmp = new double[_n + 1];
			Sx=0.0; Sy=0.0; Sxy=0.0; Sz=0.0; xVal0=0.0; tmp =0.0;

			for (i = 1; i <= _n; i++) {

				tmp =_elt._val[i]*y._elt._val[i];
				Sz += tmp;
				Sxy += fabs(tmp);
				Sx += fabs(_elt._val[i]);
				Sy += fabs(y._elt._val[i]);

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (i = 0; i <= _n; i++) {
				_elt._val[i] *= y._elt._val[0];
			}

			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (i = 1; i <= _n; i++) {
				xTmp[i] = xVal0*y._elt._val[i];
			}

			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (i = 0; i <= _n; i++) {
				_elt._val[i] += xTmp[i];
			}

			_elt._val[0] += 0.5 *Sz ;
			_elt._err = fabs(y._elt._val[0]) * _elt._err + fabs(xVal0) * y._elt._err +(_elt._err+Sx)*(y._elt._err+Sy)- 0.5 *  Sxy;


			{
				bool b = (_elt._err<POS_INFINITY);
				for (i=0;i<=_n;i++) {
					b &= (fabs(_elt._val[i])<POS_INFINITY);
				}
				if (!b) {
					*this = Interval::ALL_REALS;
				}
			}
			delete[] xTmp;

		} else {
			if (_n>y.size()) {
				*this *= y.itv();
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *= tmp1;
			}
		}


	} else {
		*this = itv()*y.itv();
	}
//	std::cout << "out *= "<< *this<<std::endl;

	return *this;
}



template<>
Affine2Main<AF_No>& Affine2Main<AF_No>::sqr(const Interval itv) {
//	std::cout << "in sqr "<<std::endl;

	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC())  ) {
		*this = pow(itv,2);

	} else  {

		double Sx, Sx2, x0;
		Sx = 0; Sx2 = 0;  x0 = 0;

		// compute the error
		for (int i = 1; i <= _n; i++) {
			Sx2 += _elt._val[i]*_elt._val[i];
			Sx += fabs(_elt._val[i]);
		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];
		_elt._val[0] = x0*x0;

		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			_elt._val[i] *= (2*x0);
		}
		_elt._val[0] += 0.5 *Sx2;

		_elt._err = 2*fabs(x0) * _elt._err-0.5 *Sx2  + __builtin_powi((_elt._err+Sx),2);


		{
			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	}

//	std::cout << "out sqr "<<std::endl;
	return *this;
}




}// end namespace ibex




