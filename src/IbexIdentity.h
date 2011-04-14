/*---------------------------------------------------------------------------------
 * 
 * Identity contractor
 * -------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_IDENTITY_H
#define _IBEX_IDENTITY_H

#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Identity contractor.
 *
 * The one that does nothing (for all box [x], C([x])=[x])
 *
 * \author Gilles Chabert
 * \date Oct 2010
 */
class Identity : public Propagation {
  public:
  /** Create an identity contractor */
  Identity();

  /** Duplicate this instance. */
  Identity(const Identity& id);

  /** Delete this instance. */
  ~Identity();
  
  /** Implement the contractor abstract copy.*/
  Identity* copy() const { return new Identity(*this); }
  
  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& op) const {
    op.print(*this);
  }

};

}

#endif