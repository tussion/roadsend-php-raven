/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#ifndef RPHP_DUMPAST_H_
#define RPHP_DUMPAST_H_

#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pBaseVisitor.h"

class TiXmlDocument;
class TiXmlElement;

namespace rphp { namespace AST { namespace Pass {

class DumpAST: public pBaseVisitor {

    TiXmlDocument*doc_;
    TiXmlElement* currentElement_;

public:
    DumpAST(pSourceModule* m):
            pBaseVisitor("AST Dump","Basic dump of the AST", m),
            doc_(NULL),
            currentElement_(NULL)
            { }

    void pre_run(void);
    void post_run(void);

    void visit_pre_stmt(stmt*);
    void visit_post_stmt(stmt*);

    void visit_pre_var(var*);
    void visit_pre_unaryOp(unaryOp* n);
    void visit_pre_binaryOp(binaryOp* n);
    void visit_pre_literalString(literalString* n);

};

} } } // namespace

#endif /* RPHP_DUMPAST_H_ */
