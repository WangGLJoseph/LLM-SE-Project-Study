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
#include "sp/node/TNode.h"

class PKB {
 private:
  std::shared_ptr<PkbWriter> pkbWriter;
  std::shared_ptr<PkbReader> pkbReader;
  std::shared_ptr<EntityManager> entityManager;
  std::shared_ptr<PatternManager> patternManager;
  std::shared_ptr<RelationshipManager> relationshipManager;
 public:
  PKB();
  // ai-gen start (copilot, 2)
  inline const std::shared_ptr<PkbWriter> &getPkbWriter() {
    return pkbWriter;
  }

  // ai-gen stop
  inline const std::shared_ptr<PkbReader> &getPkbReader() {
    return pkbReader;
  }
};
