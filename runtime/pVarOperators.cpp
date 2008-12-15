/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#include "rphp/runtime/pTypeOperators.h"
#include "rphp/runtime/pVarOperators.h"

namespace rphp {

pVar pVar_add(const pVar &lhs, const pVar &rhs)
{
    pVar result;
    
    if ( (lhs.getType() == pVarHashType) && (rhs.getType() == pVarHashType) ) {
        // TODO: array concat
        result = 0l;
    }
    else {
        // TODO: handle floats, automatic overflow, etc
        result = lhs.copyAsInt() + rhs.copyAsInt();
    }

    return result;
}


} /* namespace rphp */

