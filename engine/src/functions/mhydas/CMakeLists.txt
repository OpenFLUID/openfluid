CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


# TODO activate this

ADD_FUNCTION_WITH_DOC(water.surf.transfer-rs.hayami water.surf.transfer-rs.hayami HayamiRSFunc.cpp mhydas)
ADD_FUNCTION_WITH_DOC(water.surf.transfer-su.hayami water.surf.transfer-su.hayami HayamiSUFunc.cpp mhydas)
ADD_FUNCTION_WITH_DOC(water.surf.transfer-rs-exchange-gu.hayami-tank water.surf.transfer-rs-exchange-gu.hayami-tank HayamiRSGUFunc.cpp mhydas)
ADD_FUNCTION_WITH_DOC(water.surf-uz.runoff-infiltration.mseytoux water.surf-uz.runoff-infiltration.mseytoux MSeytouxFunc.cpp mhydas)

