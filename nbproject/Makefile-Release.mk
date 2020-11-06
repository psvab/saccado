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
	${OBJECTDIR}/Core/Component.o \
	${OBJECTDIR}/Core/ComponentManager.o \
	${OBJECTDIR}/Core/EventQ.o \
	${OBJECTDIR}/Core/Logger.o \
	${OBJECTDIR}/Core/Network.o \
	${OBJECTDIR}/Exchg/MDLink.o \
	${OBJECTDIR}/Exchg/TradingLink.o \
	${OBJECTDIR}/Instref/Instref.o \
	${OBJECTDIR}/Strategy/Strategy.o \
	${OBJECTDIR}/Util/MDSender.o \
	${OBJECTDIR}/Util/MockExchange.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/gtests.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++17
CXXFLAGS=-std=c++17

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lgtest -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sctrade

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sctrade: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sctrade ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Core/Component.o: Core/Component.cpp
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Component.o Core/Component.cpp

${OBJECTDIR}/Core/ComponentManager.o: Core/ComponentManager.cpp
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/ComponentManager.o Core/ComponentManager.cpp

${OBJECTDIR}/Core/EventQ.o: Core/EventQ.cpp
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/EventQ.o Core/EventQ.cpp

${OBJECTDIR}/Core/Logger.o: Core/Logger.cpp
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Logger.o Core/Logger.cpp

${OBJECTDIR}/Core/Network.o: Core/Network.cpp
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Network.o Core/Network.cpp

${OBJECTDIR}/Exchg/MDLink.o: Exchg/MDLink.cpp
	${MKDIR} -p ${OBJECTDIR}/Exchg
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Exchg/MDLink.o Exchg/MDLink.cpp

${OBJECTDIR}/Exchg/TradingLink.o: Exchg/TradingLink.cpp
	${MKDIR} -p ${OBJECTDIR}/Exchg
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Exchg/TradingLink.o Exchg/TradingLink.cpp

${OBJECTDIR}/Instref/Instref.o: Instref/Instref.cpp
	${MKDIR} -p ${OBJECTDIR}/Instref
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Instref/Instref.o Instref/Instref.cpp

${OBJECTDIR}/Strategy/Strategy.o: Strategy/Strategy.cpp
	${MKDIR} -p ${OBJECTDIR}/Strategy
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Strategy/Strategy.o Strategy/Strategy.cpp

${OBJECTDIR}/Util/MDSender.o: Util/MDSender.cpp
	${MKDIR} -p ${OBJECTDIR}/Util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Util/MDSender.o Util/MDSender.cpp

${OBJECTDIR}/Util/MockExchange.o: Util/MockExchange.cpp
	${MKDIR} -p ${OBJECTDIR}/Util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Util/MockExchange.o Util/MockExchange.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/gtests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/gtests.o: tests/gtests.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -I. -include tests/componentmanagertest.h -include tests/ringbuffertest.h -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/gtests.o tests/gtests.cpp


${OBJECTDIR}/Core/Component_nomain.o: ${OBJECTDIR}/Core/Component.o Core/Component.cpp 
	${MKDIR} -p ${OBJECTDIR}/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Core/Component.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Component_nomain.o Core/Component.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Core/Component.o ${OBJECTDIR}/Core/Component_nomain.o;\
	fi

${OBJECTDIR}/Core/ComponentManager_nomain.o: ${OBJECTDIR}/Core/ComponentManager.o Core/ComponentManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Core/ComponentManager.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/ComponentManager_nomain.o Core/ComponentManager.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Core/ComponentManager.o ${OBJECTDIR}/Core/ComponentManager_nomain.o;\
	fi

${OBJECTDIR}/Core/EventQ_nomain.o: ${OBJECTDIR}/Core/EventQ.o Core/EventQ.cpp 
	${MKDIR} -p ${OBJECTDIR}/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Core/EventQ.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/EventQ_nomain.o Core/EventQ.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Core/EventQ.o ${OBJECTDIR}/Core/EventQ_nomain.o;\
	fi

${OBJECTDIR}/Core/Logger_nomain.o: ${OBJECTDIR}/Core/Logger.o Core/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Core/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Logger_nomain.o Core/Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Core/Logger.o ${OBJECTDIR}/Core/Logger_nomain.o;\
	fi

${OBJECTDIR}/Core/Network_nomain.o: ${OBJECTDIR}/Core/Network.o Core/Network.cpp 
	${MKDIR} -p ${OBJECTDIR}/Core
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Core/Network.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/Network_nomain.o Core/Network.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Core/Network.o ${OBJECTDIR}/Core/Network_nomain.o;\
	fi

${OBJECTDIR}/Exchg/MDLink_nomain.o: ${OBJECTDIR}/Exchg/MDLink.o Exchg/MDLink.cpp 
	${MKDIR} -p ${OBJECTDIR}/Exchg
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Exchg/MDLink.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Exchg/MDLink_nomain.o Exchg/MDLink.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Exchg/MDLink.o ${OBJECTDIR}/Exchg/MDLink_nomain.o;\
	fi

${OBJECTDIR}/Exchg/TradingLink_nomain.o: ${OBJECTDIR}/Exchg/TradingLink.o Exchg/TradingLink.cpp 
	${MKDIR} -p ${OBJECTDIR}/Exchg
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Exchg/TradingLink.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Exchg/TradingLink_nomain.o Exchg/TradingLink.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Exchg/TradingLink.o ${OBJECTDIR}/Exchg/TradingLink_nomain.o;\
	fi

${OBJECTDIR}/Instref/Instref_nomain.o: ${OBJECTDIR}/Instref/Instref.o Instref/Instref.cpp 
	${MKDIR} -p ${OBJECTDIR}/Instref
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Instref/Instref.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Instref/Instref_nomain.o Instref/Instref.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Instref/Instref.o ${OBJECTDIR}/Instref/Instref_nomain.o;\
	fi

${OBJECTDIR}/Strategy/Strategy_nomain.o: ${OBJECTDIR}/Strategy/Strategy.o Strategy/Strategy.cpp 
	${MKDIR} -p ${OBJECTDIR}/Strategy
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Strategy/Strategy.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Strategy/Strategy_nomain.o Strategy/Strategy.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Strategy/Strategy.o ${OBJECTDIR}/Strategy/Strategy_nomain.o;\
	fi

${OBJECTDIR}/Util/MDSender_nomain.o: ${OBJECTDIR}/Util/MDSender.o Util/MDSender.cpp 
	${MKDIR} -p ${OBJECTDIR}/Util
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Util/MDSender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Util/MDSender_nomain.o Util/MDSender.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Util/MDSender.o ${OBJECTDIR}/Util/MDSender_nomain.o;\
	fi

${OBJECTDIR}/Util/MockExchange_nomain.o: ${OBJECTDIR}/Util/MockExchange.o Util/MockExchange.cpp 
	${MKDIR} -p ${OBJECTDIR}/Util
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Util/MockExchange.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Util/MockExchange_nomain.o Util/MockExchange.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Util/MockExchange.o ${OBJECTDIR}/Util/MockExchange_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Igtest -ICore -I. -IExchg -IInstref -IUtil -IStrategy -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
