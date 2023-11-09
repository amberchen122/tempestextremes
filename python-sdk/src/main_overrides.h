#define main DetectNodes_main
#define SatisfiesThreshold DetectNodes_SatisfiesThreshold // SatistiesThreshold is defined in both DetectNodes.cpp and DetectBlobs.cpp
#include "../src/nodes/DetectNodes.cpp"
#undef SatisfiesThreshold
#undef main

#define main StitchNodes_main
#define Node StitchNodes_Node // Node is defined in both StitchNodes.cpp and GridElements.cpp
#include "../src/nodes/StitchNodes.cpp"
#undef Node
#undef main

#define main AccumulateData_main
#include "../src/util/AccumulateData.cpp"
#undef main

#define main AccumulateERA5Forecast_main
#include "../src/util/AccumulateERA5Forecast.cpp"
#undef main

#define main BlobStats_main
#include "../src/blobs/BlobStats.cpp"
#undef main

#define main Climatology_main
#include "../src/util/Climatology.cpp"
#undef main

#define main CompressBlobs_main
#include "../src/util/CompressBlobs.cpp"
#undef main

#define main DetectBlobs_main
#define FilterOp DetectBlobs_FilterOp // FilterOp is defined in both DetectBlobs.cpp and NodeFileEditor.cpp
#include "../src/blobs/DetectBlobs.cpp"
#undef FilterOp
#undef main

#define main FourierFilter_main
#include "../src/util/FourierFilter.cpp"
#undef main

#define main GenerateConnectivityFile_main
#include "../src/util/GenerateConnectivityFile.cpp"
#undef main

#define main GenerateNearestNeighborMap_main
#include "../src/util/GenerateNearestNeighborMap.cpp"
#undef main

#define main HistogramNodes_main
#include "../src/nodes/HistogramNodes.cpp"
#undef main

#define main IntegrateDimension_main
#include "../src/util/IntegrateDimension.cpp"
#undef main

#define main LagrangianParcelTracker_main
#include "../src/util/LagrangianParcelTracker.cpp"
#undef main

#define main NodeFileCompose_main
#include "../src/nodes/NodeFileCompose.cpp"
#undef main

#define main NodeFileEditor_main
#include "../src/nodes/NodeFileEditor.cpp"
#undef main

#define main PersistentBlobs_main
#include "../src/blobs/PersistentBlobs.cpp"
#undef main

#define main QuantileCalculator_main
#include "../src/util/QuantileCalculator.cpp"
#undef main

#define main ShapefileMask_main
#include "../src/util/ShapefileMask.cpp"
#undef main

#define main SpineARs_main
#include "../src/atmrivers/SpineARs.cpp"
#undef main

#define main StitchBlobs_main
#include "../src/blobs/StitchBlobs.cpp"
#undef main

#define main VariableProcessor_main
#include "../src/util/VariableProcessor.cpp"
#undef main
