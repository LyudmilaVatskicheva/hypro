/* 
 * File:   ReachTree.h
 * Author: Dustin Hütter <dustin.huetter@rwth-aachen.de>
 *
 * Created on April 8, 2016, 12:04 PM
 */

#include "ReachTreeNode.h"

namespace hypro
{    
    template <typename Number, typename Representation>
    class ReachTree
    {
        private:
            ReachTreeNode* mRoot;
            
        public:
            ReachTree( ReachTreeNode* _root );
    };
}    

