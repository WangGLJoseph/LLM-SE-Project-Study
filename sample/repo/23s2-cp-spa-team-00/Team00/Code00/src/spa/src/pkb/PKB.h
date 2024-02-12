#pragma once

#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "managers/EntityManager.h"
#include "managers/PatternManager.h"
#include "managers/RelationshipManager.h"
#include "PkbReader.h"
#include "PkbWriter.h"

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
 private:
    // ai-gen start(gpt,1, e)
    // prompt: https://platform.openai.com/playground/p/jAathSkacpXnWFzmjHDfMGe6?model=gpt-4&mode=chat
    std::shared_ptr<PkbWriter> pkbWriter;
    std::shared_ptr<PkbReader> pkbReader;
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<PatternManager> patternManager;
    std::shared_ptr<RelationshipManager> relationshipManager;

    // ai-gen end

 public:
    PKB();
    static VarTable* varTable;
    static int setProcToAST(PROC p, TNode* r);
    static TNode* getRootAST (PROC p);

    // ai-gen      start(  copilot ,   2 , c   )
    // prompt: used copilot
    inline const std::shared_ptr<PkbWriter> &getPkbWriter() {
        return pkbWriter;
    }
    // ai-gen end

    inline const std::shared_ptr<PkbReader> &getPkbReader() {
        return pkbReader;
    }
};
