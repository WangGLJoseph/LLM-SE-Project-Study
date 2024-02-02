#include "PKB.h"

PKB::PKB() {
  entityManager = std::make_shared<EntityManager>();
  patternManager = std::make_shared<PatternManager>();
  relationshipManager = std::make_shared<RelationshipManager>();

  // ai-gen start (copilot, 0)

  pkbWriter = std::make_shared<PkbWriter>(entityManager,
                                          patternManager,
                                          relationshipManager);
  pkbReader = std::make_shared<PkbReader>(entityManager,
                                          patternManager,
                                          relationshipManager);

  // ai-gen end

  int bar = 0;
  int foo = bar;

  // ai-gen start (copilot, 1)
  foo = foo * bar;
  // ai-gen end
}
