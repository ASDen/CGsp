#ifndef COOKING_H

#define COOKING_H

#include "NxCooking.h"

class NxPMap;
class NxTriangleMesh;
class NxUserOutputStream;

//#pragma message(CGSP_CC)
CGSP_CC bool hasCookingLibrary(); // check to see if the cooking library is available or not!
CGSP_CC bool InitCooking(NxUserAllocator* allocator = NULL, NxUserOutputStream* outputStream = NULL);
CGSP_CC void CloseCooking();
CGSP_CC bool CookConvexMesh(const NxConvexMeshDesc& desc, NxStream& stream);
CGSP_CC bool CookClothMesh(const NxClothMeshDesc& desc, NxStream& stream);
CGSP_CC bool CookTriangleMesh(const NxTriangleMeshDesc& desc, NxStream& stream);
CGSP_CC bool CookSoftBodyMesh(const NxSoftBodyMeshDesc& desc, NxStream& stream);
CGSP_CC bool CreatePMap(NxPMap& pmap, const NxTriangleMesh& mesh, NxU32 density, NxUserOutputStream* outputStream = NULL);
CGSP_CC bool ReleasePMap(NxPMap& pmap);


#endif
