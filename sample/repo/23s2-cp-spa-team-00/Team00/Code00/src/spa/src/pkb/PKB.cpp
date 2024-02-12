#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "../TNode.h"

PKB::PKB() {
    entityManager = std::make_shared<EntityManager>();
    patternManager = std::make_shared<PatternManager>();
    relationshipManager = std::make_shared<RelationshipManager>();

    // ai-gen start (copilot, 0, c)
    // prompt:used copilot
    pkbWriter = std::make_shared<PkbWriter>(entityManager,
                                            patternManager,
                                            relationshipManager);
    pkbReader = std::make_shared<PkbReader>(entityManager,
                                            patternManager,
                                            relationshipManager);
    // ai-gen end

    int bar = 0;
    int foo = bar;

    // ai-gen start (copilot,1,e)
    // prompt: used copilot
    foo = foo * bar;
    // ai-gen end
}

int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p) {
	return nullptr;
}
