###
#
# Made by: Julien Fouilhé <julien.fouilhe@gmail.com>
# This Makefile reads dependencies from files and recompiles files
# which are dependant of a modified file.
# It also moves object files to a hidden directory.
#
###

CXX = g++

CXXFLAGS = -W -Wall -Werror -Wextra -pedantic -I. -I../include -c -std=c++11 -O2 \
					 -include ../include/AdaptersFactory.hh \
					 -Dnblog="BOOST_LOG(AdaptersFactory::getInstance()->logger()) << \"$(shell dirname $<) adapter: \"" \
					 -DBOOST_ALL_DYN_LINK

BOOST_SUFFIX = -mt

BIN_DIR = bin

SRCDIRS := $(shell find . -name '*.cpp' -exec dirname {} \; | uniq)
OBJDIR = ../.dobjects

SRCS := $(wildcard */*.cpp)
OBJS := $(patsubst %.cpp,$(OBJDIR)/adapters/%.o,$(SRCS))

DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

all: buildrepo $(OBJS)

$(OBJDIR)/adapters/%.o: %.cpp
	@echo "Generating adapter dependencies for $<"
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@echo "Erasing adapters objects files"
	@$(RM) -r $(OBJDIR)/adapters

buildrepo:
	@$(call make-repo)

define make-repo
for dir in $(SRCDIRS); \
do \
mkdir -p $(OBJDIR)/adapters/$$dir; \
done
endef

define make-depend
$(CXX) -MM \
-MF $3 \
-MP \
-MT $2 \
$(CXXFLAGS) \
$(INCLUDES) \
$1
endef

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif

re: fclean all
