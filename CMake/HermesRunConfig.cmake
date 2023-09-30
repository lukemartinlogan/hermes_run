# Find HermesRun header and library.
#

# This module defines the following uncached variables:
#  HermesRun_FOUND, if false, do not try to use HermesRun.
#  HermesRun_INCLUDE_DIRS, where to find HermesRun.h.
#  HermesRun_LIBRARIES, the libraries to link against to use the HermesRun library
#  HermesRun_LIBRARY_DIRS, the directory where the HermesRun library is found.

find_path(
  HermesRun_INCLUDE_DIR
        hermes_run/hermes_run_types.h
)

if( HermesRun_INCLUDE_DIR )
  get_filename_component(HermesRun_DIR ${HermesRun_INCLUDE_DIR} PATH)

  #-----------------------------------------------------------------------------
  # Find all packages needed by HermesRun
  #-----------------------------------------------------------------------------
  find_library(
    HermesRun_LIBRARY
    NAMES hermes_run_client hermes_run_runtime
  )

  # HermesRunShm
  find_package(HermesRunShm CONFIG REQUIRED)
  message(STATUS "found hermes_shm.h at ${HermesRunShm_INCLUDE_DIRS}")

  # YAML-CPP
  find_package(yaml-cpp REQUIRED)
  message(STATUS "found yaml-cpp at ${yaml-cpp_DIR}")

  # Catch2
  find_package(Catch2 3.0.1 REQUIRED)
  message(STATUS "found catch2.h at ${Catch2_CXX_INCLUDE_DIRS}")

  # MPICH
  if(BUILD_MPI_TESTS)
    find_package(MPI REQUIRED COMPONENTS C CXX)
    message(STATUS "found mpi.h at ${MPI_CXX_INCLUDE_DIRS}")
  endif()

  # OpenMP
  if(BUILD_OpenMP_TESTS)
    find_package(OpenMP REQUIRED COMPONENTS C CXX)
    message(STATUS "found omp.h at ${OpenMP_CXX_INCLUDE_DIRS}")
  endif()

  # Cereal
  find_package(cereal REQUIRED)
  if(cereal)
    message(STATUS "found cereal")
  endif()

  # Boost
  find_package(Boost REQUIRED COMPONENTS regex system filesystem fiber REQUIRED)
  if (Boost_FOUND)
    message(STATUS "found boost at ${Boost_INCLUDE_DIRS}")
  endif()

  # Thallium
  find_package(thallium CONFIG REQUIRED)
  if(thallium_FOUND)
    message(STATUS "found thallium at ${thallium_DIR}")
  endif()

  #-----------------------------------------------------------------------------
  # Mark hermes as found and set all needed packages
  #-----------------------------------------------------------------------------
  if( HermesRun_LIBRARY )
    set(HermesRun_LIBRARY_DIR "")
    get_filename_component(HermesRun_LIBRARY_DIRS ${HermesRun_LIBRARY} PATH)
    # Set uncached variables as per standard.
    set(HermesRun_FOUND ON)
    set(HermesRun_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${HermesRun_INCLUDE_DIR})
    set(HermesRun_LIBRARIES
            ${HermesRunShm_LIBRARIES}
            yaml-cpp
            cereal::cereal
            -ldl -lrt -lc -pthread
            thallium
            ${Boost_LIBRARIES} ${HermesRun_LIBRARY})
    set(HermesRun_CLIENT_LIBRARIES ${HermesRun_LIBRARIES})
  endif(HermesRun_LIBRARY)

else(HermesRun_INCLUDE_DIR)
  message(STATUS "FindHermesRun: Could not find HermesRun.h")
endif(HermesRun_INCLUDE_DIR)
	    
if(HermesRun_FOUND)
  if(NOT HermesRun_FIND_QUIETLY)
    message(STATUS "FindHermesRun: Found both HermesRun.h and libHermesRun_client.so")
  endif(NOT HermesRun_FIND_QUIETLY)
else(HermesRun_FOUND)
  if(HermesRun_FIND_REQUIRED)
    message(STATUS "FindHermesRun: Could not find HermesRun.h and/or libHermesRun_client.so")
  endif(HermesRun_FIND_REQUIRED)
endif(HermesRun_FOUND)
