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
#include <iomanip>
#include <fstream>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include "parser/phplexer.h"
#include "parser/rphp_parser.h"
#include "pDriver.h"

using namespace std;

namespace rphp {

    /**
    * print the token with the same text as php tokens - so they can be compared with
    * the result of get_token_all (see test-tokenize.php)
    **/
    void printToken(int token, const Lexer& lexer, const UnicodeString& content)
    {
        int begin = lexer.tokenBegin();
        int end = lexer.tokenEnd();
        //UnicodeString tokenText = content.replace(begin, end-begin+1,"\n", "\\n");
        UnicodeString tokenText(content, begin, end-begin+1);
        if (token == parser::Token_INLINE_HTML) {
            cout << tokenText << " T_INLINE_HTML" << endl;
        } else if (token == parser::Token_OPEN_TAG) {
            cout << tokenText << " T_OPEN_TAG" << endl;
        } else if (token == parser::Token_CLOSE_TAG) {
            cout << tokenText << " T_CLOSE_TAG" << endl;
        } else if (token == parser::Token_ECHO) {
            cout << tokenText << " T_ECHO" << endl;
        } else if (token == parser::Token_WHITESPACE) {
            cout << tokenText << " T_WHITESPACE" << endl;
        } else if (token == parser::Token_CONSTANT_ENCAPSED_STRING) {
            cout << tokenText << " T_CONSTANT_ENCAPSED_STRING" << endl;
        } else if (token == parser::Token_SEMICOLON) {
            cout << tokenText << " ;" << endl;
        } else if (token == parser::Token_VARIABLE) {
            cout << tokenText << " T_VARIABLE" << endl;
        } else if (token == parser::Token_DOUBLE_QUOTE) {
            cout << tokenText << " \"" << endl;
        } else if (token == parser::Token_ENCAPSED_AND_WHITESPACE) {
            cout << tokenText << " T_ENCAPSED_AND_WHITESPACE" << endl;
        } else if (token == parser::Token_OBJECT_OPERATOR) {
            cout << tokenText << " T_OBJECT_OPERATOR" << endl;
        } else if (token == parser::Token_LBRACKET) {
            cout << tokenText << " [" << endl;
        } else if (token == parser::Token_RBRACKET) {
            cout << tokenText << " ]" << endl;
        } else if (token == parser::Token_NUM_STRING) {
            cout << tokenText << " T_NUM_STRING" << endl;
        } else if (token == parser::Token_STRING) {
            cout << tokenText << " T_STRING" << endl;
        } else if (token == parser::Token_ASSIGN) {
            cout << tokenText << " =" << endl;
        } else if (token == parser::Token_DNUMBER) {
            cout << tokenText << " T_DNUMBER" << endl;
        } else if (token == parser::Token_LNUMBER) {
            cout << tokenText << " T_LNUMBER" << endl;
        } else if (token == parser::Token_PLUS) {
            cout << tokenText << " +" << endl;
        } else if (token == parser::Token_MINUS) {
            cout << tokenText << " -" << endl;
        } else if (token == parser::Token_CONCAT) {
            cout << tokenText << " ." << endl;
        } else if (token == parser::Token_INC) {
            cout << tokenText << " T_INC" << endl;
        } else if (token == parser::Token_DEC) {
            cout << tokenText << " T_DEC" << endl;
        } else if (token == parser::Token_IS_EQUAL) {
            cout << tokenText << " T_IS_EQUAL" << endl;
        } else if (token == parser::Token_IS_NOT_EQUAL) {
            cout << tokenText << " T_IS_NOT_EQUAL" << endl;
        } else if (token == parser::Token_IS_IDENTICAL) {
            cout << tokenText << " T_IS_IDENTICAL" << endl;
        } else if (token == parser::Token_IS_NOT_IDENTICAL) {
            cout << tokenText << " T_IS_NOT_IDENTICAL" << endl;
        } else if (token == parser::Token_IS_SMALLER) {
            cout << tokenText << " <" << endl;
        } else if (token == parser::Token_IS_GREATER) {
            cout << tokenText << " >" << endl;
        } else if (token == parser::Token_IS_SMALLER_OR_EQUAL) {
            cout << tokenText << " T_IS_SMALLER_OR_EQUAL" << endl;
        } else if (token == parser::Token_IS_GREATER_OR_EQUAL) {
            cout << tokenText << " T_IS_GREATER_OR_EQUAL" << endl;
        } else if (token == parser::Token_BOOLEAN_OR) {
            cout << tokenText << " T_BOOLEAN_OR" << endl;
        } else if (token == parser::Token_BOOLEAN_AND) {
            cout << tokenText << " T_BOOLEAN_AND" << endl;
        } else if (token == parser::Token_PLUS_ASSIGN) {
            cout << tokenText << " T_PLUS_EQUAL" << endl;
        } else if (token == parser::Token_MINUS_ASSIGN) {
            cout << tokenText << " T_MINUS_EQUAL" << endl;
        } else if (token == parser::Token_MUL_ASSIGN) {
            cout << tokenText << " T_MUL_EQUAL" << endl;
        } else if (token == parser::Token_DIV_ASSIGN) {
            cout << tokenText << " T_DIV_EQUAL" << endl;
        } else if (token == parser::Token_CONCAT_ASSIGN) {
            cout << tokenText << " T_CONCAT_EQUAL" << endl;
        } else if (token == parser::Token_MOD_ASSIGN) {
            cout << tokenText << " T_MOD_EQUAL" << endl;
        } else if (token == parser::Token_AND_ASSIGN) {
            cout << tokenText << " T_AND_EQUAL" << endl;
        } else if (token == parser::Token_OR_ASSIGN) {
            cout << tokenText << " T_OR_EQUAL" << endl;
        } else if (token == parser::Token_XOR_ASSIGN) {
            cout << tokenText << " T_XOR_EQUAL" << endl;
        } else if (token == parser::Token_SL_ASSIGN) {
            cout << tokenText << " T_SL_EQUAL" << endl;
        } else if (token == parser::Token_SR_ASSIGN) {
            cout << tokenText << " T_SR_EQUAL" << endl;
        } else if (token == parser::Token_BANG) {
            cout << tokenText << " !" << endl;
        } else if (token == parser::Token_QUESTION) {
            cout << tokenText << " ?" << endl;
        } else if (token == parser::Token_COLON) {
            cout << tokenText << " :" << endl;
        } else if (token == parser::Token_BIT_AND) {
            cout << tokenText << " &" << endl;
        } else if (token == parser::Token_BIT_OR) {
            cout << tokenText << " |" << endl;
        } else if (token == parser::Token_BIT_XOR) {
            cout << tokenText << " ^" << endl;
        } else if (token == parser::Token_SL) {
            cout << tokenText << " T_SL" << endl;
        } else if (token == parser::Token_SR) {
            cout << tokenText << " T_SR" << endl;
        } else if (token == parser::Token_MUL) {
            cout << tokenText << " *" << endl;
        } else if (token == parser::Token_DIV) {
            cout << tokenText << " /" << endl;
        } else if (token == parser::Token_MOD) {
            cout << tokenText << " %" << endl;
        } else if (token == parser::Token_TILDE) {
            cout << tokenText << " ~" << endl;
        } else if (token == parser::Token_LPAREN) {
            cout << tokenText << " (" << endl;
        } else if (token == parser::Token_RPAREN) {
            cout << tokenText << " )" << endl;
        } else if (token == parser::Token_LBRACE) {
            cout << tokenText << " {" << endl;
        } else if (token == parser::Token_RBRACE) {
            cout << tokenText << " }" << endl;
        } else if (token == parser::Token_COMMA) {
            cout << tokenText << " ," << endl;
        } else if (token == parser::Token_AT) {
            cout << tokenText << " @" << endl;
        } else if (token == parser::Token_INCLUDE) {
            cout << tokenText << " T_INCLUDE" << endl;
        } else if (token == parser::Token_INCLUDE_ONCE) {
            cout << tokenText << " T_INCLUDE_ONCE" << endl;
        } else if (token == parser::Token_EVAL) {
            cout << tokenText << " T_EVAL" << endl;
        } else if (token == parser::Token_REQUIRE) {
            cout << tokenText << " T_REQUIRE" << endl;
        } else if (token == parser::Token_REQUIRE_ONCE) {
            cout << tokenText << " T_REQUIRE_ONCE" << endl;
        } else if (token == parser::Token_PRINT) {
            cout << tokenText << " T_PRINT" << endl;
        } else if (token == parser::Token_ABSTRACT) {
            cout << tokenText << " T_ABSTRACT" << endl;
        } else if (token == parser::Token_BREAK) {
            cout << tokenText << " T_BREAK" << endl;
        } else if (token == parser::Token_CASE) {
            cout << tokenText << " T_CASE" << endl;
        } else if (token == parser::Token_CATCH) {
            cout << tokenText << " T_CATCH" << endl;
        } else if (token == parser::Token_CLASS) {
            cout << tokenText << " T_CLASS" << endl;
        } else if (token == parser::Token_CONST) {
            cout << tokenText << " T_CONST" << endl;
        } else if (token == parser::Token_CONTINUE) {
            cout << tokenText << " T_CONTINUE" << endl;
        } else if (token == parser::Token_DEFAULT) {
            cout << tokenText << " T_DEFAULT" << endl;
        } else if (token == parser::Token_DO) {
            cout << tokenText << " T_DO" << endl;
        } else if (token == parser::Token_ELSE) {
            cout << tokenText << " T_ELSE" << endl;
        } else if (token == parser::Token_EXTENDS) {
            cout << tokenText << " T_EXTENDS" << endl;
        } else if (token == parser::Token_FINAL) {
            cout << tokenText << " T_FINAL" << endl;
        } else if (token == parser::Token_FOR) {
            cout << tokenText << " T_FOR" << endl;
        } else if (token == parser::Token_IF) {
            cout << tokenText << " T_IF" << endl;
        } else if (token == parser::Token_IMPLEMENTS) {
            cout << tokenText << " T_IMPLEMENTS" << endl;
        } else if (token == parser::Token_INSTANCEOF) {
            cout << tokenText << " T_INSTANCEOF" << endl;
        } else if (token == parser::Token_INTERFACE) {
            cout << tokenText << " T_INTERFACE" << endl;
        } else if (token == parser::Token_NEW) {
            cout << tokenText << " T_NEW" << endl;
        } else if (token == parser::Token_PRIVATE) {
            cout << tokenText << " T_PRIVATE" << endl;
        } else if (token == parser::Token_PROTECTED) {
            cout << tokenText << " T_PROTECTED" << endl;
        } else if (token == parser::Token_PUBLIC) {
            cout << tokenText << " T_PUBLIC" << endl;
        } else if (token == parser::Token_RETURN) {
            cout << tokenText << " T_RETURN" << endl;
        } else if (token == parser::Token_STATIC) {
            cout << tokenText << " T_STATIC" << endl;
        } else if (token == parser::Token_SWITCH) {
            cout << tokenText << " T_SWITCH" << endl;
        } else if (token == parser::Token_THROW) {
            cout << tokenText << " T_THROW" << endl;
        } else if (token == parser::Token_TRY) {
            cout << tokenText << " T_TRY" << endl;
        } else if (token == parser::Token_WHILE) {
            cout << tokenText << " T_WHILE" << endl;
        } else if (token == parser::Token_INT_CAST) {
            cout << tokenText << " T_INT_CAST" << endl;
        } else if (token == parser::Token_DOUBLE_CAST) {
            cout << tokenText << " T_DOUBLE_CAST" << endl;
        } else if (token == parser::Token_STRING_CAST) {
            cout << tokenText << " T_STRING_CAST" << endl;
        } else if (token == parser::Token_ARRAY_CAST) {
            cout << tokenText << " T_ARRAY_CAST" << endl;
        } else if (token == parser::Token_OBJECT_CAST) {
            cout << tokenText << " T_OBJECT_CAST" << endl;
        } else if (token == parser::Token_BOOL_CAST) {
            cout << tokenText << " T_BOOL_CAST" << endl;
        } else if (token == parser::Token_UNSET_CAST) {
            cout << tokenText << " T_UNSET_CAST" << endl;
        } else if (token == parser::Token_CLONE) {
            cout << tokenText << " T_CLONE" << endl;
        } else if (token == parser::Token_EXIT) {
            cout << tokenText << " T_EXIT" << endl;
        } else if (token == parser::Token_ELSEIF) {
            cout << tokenText << " T_ELSEIF" << endl;
        } else if (token == parser::Token_ENDIF) {
            cout << tokenText << " T_ENDIF" << endl;
        } else if (token == parser::Token_ENDWHILE) {
            cout << tokenText << " T_ENDWHILE" << endl;
        } else if (token == parser::Token_ENDFOR) {
            cout << tokenText << " T_ENDFOR" << endl;
        } else if (token == parser::Token_FOREACH) {
            cout << tokenText << " T_FOREACH" << endl;
        } else if (token == parser::Token_ENDFOREACH) {
            cout << tokenText << " T_ENDFOREACH" << endl;
        } else if (token == parser::Token_DECLARE) {
            cout << tokenText << " T_DECLARE" << endl;
        } else if (token == parser::Token_ENDDECLARE) {
            cout << tokenText << " T_ENDDECLARE" << endl;
        } else if (token == parser::Token_AS) {
            cout << tokenText << " T_AS" << endl;
        } else if (token == parser::Token_ENDSWITCH) {
            cout << tokenText << " T_ENDSWITCH" << endl;
        } else if (token == parser::Token_FUNCTION) {
            cout << tokenText << " T_FUNCTION" << endl;
        } else if (token == parser::Token_USE) {
            cout << tokenText << " T_USE" << endl;
        } else if (token == parser::Token_GLOBAL) {
            cout << tokenText << " T_GLOBAL" << endl;
        } else if (token == parser::Token_VAR) {
            cout << tokenText << " T_VAR" << endl;
        } else if (token == parser::Token_UNSET) {
            cout << tokenText << " T_UNSET" << endl;
        } else if (token == parser::Token_ISSET) {
            cout << tokenText << " T_ISSET" << endl;
        } else if (token == parser::Token_ISSET) {
            cout << tokenText << " T_ISSET" << endl;
        } else if (token == parser::Token_EMPTY) {
            cout << tokenText << " T_EMPTY" << endl;
        } else if (token == parser::Token_HALT_COMPILER) {
            cout << tokenText << " T_HALT_COMPILER" << endl;
        } else if (token == parser::Token_DOUBLE_ARROW) {
            cout << tokenText << " T_DOUBLE_ARROW" << endl;
        } else if (token == parser::Token_LIST) {
            cout << tokenText << " T_LIST" << endl;
        } else if (token == parser::Token_ARRAY) {
            cout << tokenText << " T_ARRAY" << endl;
        } else if (token == parser::Token_CLASS_C) {
            cout << tokenText << " T_CLASS_C" << endl;
        } else if (token == parser::Token_METHOD_C) {
            cout << tokenText << " T_METHOD_C" << endl;
        } else if (token == parser::Token_FUNC_C) {
            cout << tokenText << " T_FUNC_C" << endl;
        } else if (token == parser::Token_LINE) {
            cout << tokenText << " T_LINE" << endl;
        } else if (token == parser::Token_FILE) {
            cout << tokenText << " T_FILE" << endl;
        } else if (token == parser::Token_COMMENT) {
            cout << tokenText << " T_COMMENT" << endl;
        } else if (token == parser::Token_DOC_COMMENT) {
            cout << tokenText << " T_DOC_COMMENT" << endl;
        } else if (token == parser::Token_PAAMAYIM_NEKUDOTAYIM) {
            cout << tokenText << " T_DOUBLE_COLON" << endl;
        } else if (token == parser::Token_OPEN_TAG_WITH_ECHO) {
            cout << tokenText << " T_OPEN_TAG_WITH_ECHO" << endl;
        } else if (token == parser::Token_CURLY_OPEN) {
            cout << tokenText << " T_CURLY_OPEN" << endl;
        } else if (token == parser::Token_STRING_VARNAME) {
            cout << tokenText << " T_STRING_VARNAME" << endl;
        } else if (token == parser::Token_DOLLAR_OPEN_CURLY_BRACES) {
            cout << tokenText << " T_DOLLAR_OPEN_CURLY_BRACES" << endl;
        } else if (token == parser::Token_DOLLAR) {
            cout << tokenText << " $" << endl;
        } else if (token == parser::Token_LOGICAL_XOR) {
            cout << tokenText << " T_LOGICAL_XOR" << endl;
        } else if (token == parser::Token_LOGICAL_AND) {
            cout << tokenText << " T_LOGICAL_AND" << endl;
        } else if (token == parser::Token_LOGICAL_OR) {
            cout << tokenText << " T_LOGICAL_OR" << endl;
        } else if (token == parser::Token_START_HEREDOC) {
            cout << tokenText << " T_START_HEREDOC" << endl;
        } else if (token == parser::Token_END_HEREDOC) {
            cout << tokenText << " T_END_HEREDOC" << endl;
        } else if (token == parser::Token_BACKTICK) {
            cout << tokenText << " `" << endl;
        } else if (token == 0) {
            cout << tokenText << " end of file" << endl;
        } else {
            cout << tokenText << " unknown token" << token;
        }
    }
    
    void pDriver::dumpTokens(string fileName) {

        ifstream inFile;
    
        inFile.open(fileName.c_str(), ifstream::in);
        if (!inFile) {
            cout << "Unable to open file: " << endl;
            exit(1); // terminate with error
        }

        UnicodeString contents;
        char buf[512];
        while (inFile) {
            inFile.getline(buf, 512);
            //cout << "read: " << buf << endl;
            contents += buf;
        }
        
        inFile.close();
        
        Lexer lexer(0, contents);
        int token;
        while ((token = lexer.nextTokenKind())) {
            printToken(token, lexer, contents);
        }
        printToken(token, lexer, contents);

    }

}

