# output bin
bins=server client

# prefix
build_prefix=build
debug_prefix=debug
run_prefix=run

build_command=python3 ./scripts/build.py
tests_command=python3 ./scripts/tests.py

#################################################
##                                             ##
##                 PHONY CONST                 ##
##                                             ##
#################################################

build_rules=$(addprefix $(build_prefix)_, $(bins))
debug_rules=$(addprefix $(debug_prefix)_, $(bins))
run_rules=$(addprefix $(run_prefix)_, $(bins))

cflag= -Wall -Wextra -Werror -std=c99 -pedantic -export-dynamic -lm
test_cflag= -Wall -Wextra -Werror -std=c99 -g

.PHONY: help clean all build debug tests tests_build tests_run $(build_rules) $(debug_rules) $(run_rules)

all: build

build: $(build_rules)
debug: $(debug_rules)

help:
	@echo "Usage: {Action}[_{Target}]"
	@echo ""
	@echo "if no {Target} is specify apply {Action} to all {Target}"
	@echo ""
	@echo "Action: "
	@echo "- help: show this help"
	@echo "- clean: remove all the bin and object files"
	@echo "- $(build_prefix): build the target"
	@echo "- $(debug_prefix): build in debug mode the target"
	@echo "- $(run_prefix): run the target"
	@echo "- tests: build and run tests"
	@echo "- tests_build: build tests"
	@echo "- tests_run: run tests"
	@echo ""
	@echo "Target: $(bins)"


clean:
	$(RM) -r obj
	$(RM) -r build

#################################################
##                                             ##
##                PHONY CREATION               ##
##                                             ##
#################################################

define bins_targets
# debug command
debug_$1:
	@echo "======================="
	@echo "   DEBUG \"$1\""
	@echo "======================="
	@$(build_command) -out=build/$1 -main=src/$1/$1.c -cflag="$(cflag) -g"
	@echo
# build command
build_$1:
	@echo "======================="
	@echo "   BUILD \"$1\""
	@echo "======================="
	@$(build_command) -out=build/$1 -main=src/$1/$1.c -cflag="$(cflag)"
	@echo

run_$1: build_$1
	@./build/$1
endef

# create the targets
$(foreach bin,$(bins),$(eval $(call bins_targets,$(bin))))

#################################################
##                                             ##
##                    TESTS                    ##
##                                             ##
#################################################

tests: tests_build tests_run

tests_build:
	@echo "======================="
	@echo "   BUILD TESTS"
	@echo "======================="
	@$(tests_command) -out build/tests -cflag="$(test_cflag)"

tests_run:
	@echo "======================="
	@echo "   RUN TESTS"
	@echo "======================="
	@find build/tests/* -maxdepth 1 -type f -executable \
	-exec echo "################################################################################" \; \
	-exec echo "" \; \
	-exec echo "                             {}" \; \
	-exec echo "" \; \
	-exec echo "################################################################################" \; \
	-exec {} \; \
	-exec echo "################################################################################" \; \
	-exec echo -e "\n\n\n" \;
