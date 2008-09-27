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

#ifndef RPHP_PLINKTARGET_H_
#define RPHP_PLINKTARGET_H_

#include <string>
#include <vector>
#include "pTarget.h"

namespace llvm {
    class Module;
}

namespace rphp {

// link targets will create various native binaries from one or more compile bitcode files
class pLinkTarget : public pTarget {

protected:
    std::string outputFile;
    std::vector<std::string> inputFiles;

    // link options (static, dynamic)

public:
    pLinkTarget(std::string outFile): outputFile(outFile) { }

    void addInputFile(std::string f) {
        inputFiles.push_back(f);
    }

    const std::string& getOutputFileName(void) { return outputFile; }

};

} // namespace

#endif /* RPHP_PLINKTARGET_H_ */
