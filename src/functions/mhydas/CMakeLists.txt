CMAKE_MINIMUM_REQUIRED (VERSION 2.6)



ADD_FUNCTION_WITH_DOC(water.surf.transfer-rs.hayami water.surf.transfer-rs.hayami HayamiRSFunc.cpp)
ADD_FUNCTION_WITH_DOC(water.surf.transfer-su.hayami water.surf.transfer-su.hayami HayamiSUFunc.cpp)
ADD_FUNCTION_WITH_DOC(water.surf.transfer-rs-exchange-gu.hayami-tank water.surf.transfer-rs-exchange-gu.hayami-tank HayamiRSGUFunc.cpp)
ADD_FUNCTION_WITH_DOC(water.surf-uz.runoff-infiltration.mseytoux water.surf-uz.runoff-infiltration.mseytoux MSeytouxFunc.cpp)

