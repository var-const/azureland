#pragma once

namespace std { template <typename T, typename Allocator> class vector; }
struct CRICollision;

typedef std::vector<CRICollision> CollisionsContT;
typedef CollisionsContT::iterator CollisionsIterT;
