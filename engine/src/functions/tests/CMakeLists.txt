CMAKE_MINIMUM_REQUIRED (VERSION 2.6)




ADD_FUNCTION(tests.vector.prod tests.vector.prod)
ADD_FUNCTION(tests.vector.use tests.vector.use)
ADD_FUNCTION(tests.largevector.prod tests.largevector.prod)
ADD_FUNCTION(tests.largevector.use tests.largevector.use)
ADD_FUNCTION(tests.primitives.prod tests.primitives.prod)
ADD_FUNCTION(tests.primitives.use tests.primitives.use)
ADD_SUBDIRECTORY(tests.fortran)
ADD_FUNCTION(tests.events tests.events)
ADD_FUNCTION(tests.loops tests.loops)


