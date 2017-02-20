#Set this to @ to keep the makefile quiet
ifndef SILENCE
        SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = asyncut
CPPUTEST_SOURCE = /home/wangxl/lxk/cpputest-3.6
CPPUTEST_HOME = /home/wangxl/lxk/cpputest-3.6

CPPUTEST_LDFLAGS =

CPPUTEST_USE_EXTENSIONS = y
CPP_PLATFORm = Gcc

CPPUTEST_USE_MEM_LEAK_DETECTION = N

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
SRC_HOME = .
SRC_DIRS = \
       $(SRC_HOME)/

TEST_SRC_DIRS = \
       $(SRC_HOME)/ut

INCLUDE_DIRS = \
       $(SRC_HOME)\
       $(CPPUTEST_HOME)/include\

include $(CPPUTEST_SOURCE)/build/MakefileWorker.mk

