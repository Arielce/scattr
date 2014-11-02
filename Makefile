###
#
# Made by: Julien Fouilhé <julien.fouilhe@gmail.com>
# This Makefile reads dependencies from files and recompiles files
# which are dependant of a modified file.
# It also moves object files to a hidden directory.
#
###

EXEC = notifier

CXX = g++

CXXFLAGS = -W -Wall -Werror -Wextra -pedantic -I. -I./include -c -std=c++11 -O2

LDFLAGS = -lboost_program_options -lboost_system -lboost_thread-mt -lcppunit -lamqpcpp

SRCDIRS := $(shell find . -name '*.cpp' -exec dirname {} \; | uniq)
OBJDIR = .dobjects

SRCS := $(wildcard src/*.cpp adapters/*/*.cpp)
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

SRCS_TESTS := $(wildcard tests/*.cpp)
OBJS_TESTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS_TESTS))
DEPS_TESTS := $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS_TESTS))

SAVES = ./.save

all: $(EXEC) test

$(EXEC): buildrepo $(OBJS)
	@echo "Building" $@
	@echo "Creating saves directory"
	@mkdir -p $(SAVES)
	@echo "$@: Linking objects files... "
	@$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	@echo "Linking done."

test: buildrepo $(OBJS) $(OBJS_TESTS)
	@echo "Building " $@
	@echo "$@: Linking objects files... "
	@$(CXX) -o $@ $(filter-out $(OBJDIR)/src/main.o, $(OBJS)) $(OBJS_TESTS) $(LDFLAGS)
	@echo "Linking done. Launching tests..."
	@echo "---------"
	@./$@
	@echo "---------"

$(OBJDIR)/%.o: %.cpp
	@echo "Generating dependencies for $<"
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@echo "Erasing objects files"
	@$(RM) -r $(OBJDIR)

fclean: clean
	@echo "Erasing executable(s)"
	@$(RM) $(EXEC)

buildrepo:
	@$(call make-repo)

define make-repo
for dir in $(SRCDIRS); \
do \
mkdir -p $(OBJDIR)/$$dir; \
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
