#define GIT_SHA1 "f9eb99f937fc2a1ba38f0d8c032c7f84a95635c7"
#define BUILD_DATE "Tue Sep 05 16:40:41 2017"
#define OPENMD_VERSION "2.6"

#include "utils/Revision.hpp"
#include <cstring>

extern "C" { 
  const char version[] = OPENMD_VERSION;
  const char revision[] = GIT_SHA1;
  const char build_date[] = BUILD_DATE;
}


namespace OpenMD {

  std::string Revision::getVersion() {
    std::string v;
    v.assign("Version: ");
    v.append(version);
    return v;
  }
  
  std::string Revision::getRevision() {
    std::string r;
    std::string rev(revision, strnlen(revision, 40));
    r.append("Revision: ");
    // If there's no GIT SHA1 revision, just call this the RELEASE revision.
    if (!rev.empty()) {
      r.append(rev);
    } else {
      r.append("RELEASE");
    }
    return r;
  }

  std::string Revision::getBuildDate() {
    std::string d;
    d.assign("Build date: ");
    d.append(build_date);
    return d;
  }
  
  std::string Revision::getFullRevision() {
    std::string fr;
    fr.assign(getVersion());
    fr.append(", ");
    fr.append(getRevision());
    return fr;
  }
}
