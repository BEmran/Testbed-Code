#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/c6ea67ce/LSM9DS1.o \
	${OBJECTDIR}/_ext/c6ea67ce/MPU9250.o \
	${OBJECTDIR}/_ext/c6ea67ce/Util.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=../../../git_ws/Navio2/C++/Navio/MPU9250.cpp ../../../git_ws/Navio2/C++/Navio/LSM9DS1.cpp ../../../git_ws/Navio2/C++/Navio/Util.cpp
CXXFLAGS=../../../git_ws/Navio2/C++/Navio/MPU9250.cpp ../../../git_ws/Navio2/C++/Navio/LSM9DS1.cpp ../../../git_ws/Navio2/C++/Navio/Util.cpp

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/navio_ahrs

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/navio_ahrs: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/navio_ahrs ${OBJECTFILES} ${LDLIBSOPTIONS} -lrt -lpthread

${OBJECTDIR}/_ext/c6ea67ce/LSM9DS1.o: ../../../git_ws/Navio2/C++/Navio/LSM9DS1.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c6ea67ce
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../git_ws/Navio2/C++ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c6ea67ce/LSM9DS1.o ../../../git_ws/Navio2/C++/Navio/LSM9DS1.cpp

${OBJECTDIR}/_ext/c6ea67ce/MPU9250.o: ../../../git_ws/Navio2/C++/Navio/MPU9250.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c6ea67ce
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../git_ws/Navio2/C++ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c6ea67ce/MPU9250.o ../../../git_ws/Navio2/C++/Navio/MPU9250.cpp

${OBJECTDIR}/_ext/c6ea67ce/Util.o: ../../../git_ws/Navio2/C++/Navio/Util.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c6ea67ce
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../git_ws/Navio2/C++ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c6ea67ce/Util.o ../../../git_ws/Navio2/C++/Navio/Util.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../git_ws/Navio2/C++ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
