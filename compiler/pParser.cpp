/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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

#include <iostream>

#include "pLexer.h"
#include "pLangParserDef.h"
#include "pParser.h"

namespace rphp { namespace parser {

pModuleP pParser::compileToAST(std::string fileName) {

    pModuleP pMod(new pModule(fileName));

    lexer::pLexer lexer(fileName);
    pLangGrammar parser(lexer.getTokens(), pMod);

    lexer::tokIteratorType iter = lexer.begin();
    lexer::tokIteratorType end = lexer.end();

    std::string ws = "WS";

    AST::treeTop moduleTop;

    bool r = phrase_parse(iter, end, parser, moduleTop, in_state(ws)[lexer.getTokens().skip_toks]);

    if (!r || iter != end) {
        std::cout << "Parsing failed\n";
    }
    

}

void pParser::dumpAST(std::string fileName) {

    pModuleP m = compileToAST(fileName);
    // apply dump visitor
    std::cout << "i should dump here" << std::endl;

}


} } // namespace
