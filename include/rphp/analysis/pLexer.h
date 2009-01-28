/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PLEXER_H_
#define RPHP_PLEXER_H_

#include "rphp_grammar.h"

#include "rphp/analysis/lexer/debug.hpp"

#include "rphp/analysis/lexer/generator.hpp"
#include "rphp/analysis/lexer/input.hpp"
#include "rphp/analysis/lexer/state_machine.hpp"

#include "rphp/analysis/pSourceFile.h"

// this is the iterator type exposed by the lexer, which dereferences to
// a token
typedef boost::lexer::iter_winput::iterator pTokenIterator;

namespace rphp { namespace lexer {

#define T_DQ_DONE        0
#define T_DQ_DQ          1
#define T_DQ_NEWLINE     2
#define T_DQ_PASSTHROUGH 3
#define T_DQ_ESCAPE      4

class pLexer {

private:

    boost::lexer::wrules langRules_;
    boost::lexer::wstate_machine langState_;
    boost::lexer::iter_winput* lexInput_;

    boost::lexer::wrules dqRules_;
    boost::lexer::wstate_machine dqState_;

    const pSourceFile* source_;
    pSourceString contents_;

    pSourceCharIterator sourceBegin_;
    pSourceCharIterator sourceEnd_;

public:

    typedef pTokenIterator iterator_type;

    pLexer(const pSourceFile* s);
    ~pLexer(void) {
        delete lexInput_;
    }

    bool preprocess(void);

    pTokenIterator tokBegin(void);
    pTokenIterator tokEnd(void);

    const pSourceCharIterator sourceBegin(void) const;
    const pSourceCharIterator sourceEnd(void) const;

    void dumpTokens(void);
    const wchar_t* getTokenDescription(const std::size_t t) const;

    const pSourceString& contents(void) const { return contents_; }

};

} } // namespace

#endif /* RPHP_PLEXER_H_ */
