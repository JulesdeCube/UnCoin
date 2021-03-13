# output bin
bins=server client

# prefix
build_prefix=build
debug_prefix=debug
run_prefix=run

build_command=python3 ./scripts/build.py

#################################################
##                                             ##
##                 PHONY CONST                 ##
##                                             ##
#################################################

build_rules=$(addprefix $(build_prefix)_, $(bins))
debug_rules=$(addprefix $(debug_prefix)_, $(bins))
run_rules=$(addprefix $(run_prefix)_, $(bins))

.PHONY: help all build debug $(build_rules) $(debug_rules) $(run_rules)

all: build

build: $(build_rules)
debug: $(debug_rules)

help:
	@echo "Usage: {Action}[_{Target}]"
	@echo ""
	@echo "if no {Target} is specify apply {Action} to all {Target}"
	@echo ""
	@echo "Action: "
	@echo "- $(build_prefix): build the target"
	@echo "- $(debug_prefix): build in debug mode the target"
	@echo "- $(run_prefix):   run the target"
	@echo ""
	@echo "Target: $(bins)"

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
	@$(build_command) -d $1
	@echo
# build command
build_$1:
	@echo "======================="
	@echo "   BUILD \"$1\""
	@echo "======================="
	@$(build_command) $1
	@echo
run_$1:
	@./build/$1
endef

# create the targets
$(foreach bin,$(bins),$(eval $(call bins_targets,$(bin))))

