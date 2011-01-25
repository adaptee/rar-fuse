/***************************************************************************
 *  Copyright (C) 2006  Kent Gustavsson <nedo80@gmail.com>
 ****************************************************************************/
 /*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


//
// Class: MarkerBlock
// Created by: Kent Gustavsson <nedo80@gmail.com>
// Created on: Mon Mar  6 21:26:40 2006
//

#ifndef _MARKERBLOCK_H_
#define _MARKERBLOCK_H_

#include "rarblock.h"

class MarkerBlock : public RARBlock
{
	public:
		MarkerBlock(std::istream &in);
		 ~MarkerBlock();
	
		
	
	protected:
		
};


#endif	//_MARKERBLOCK_H_

