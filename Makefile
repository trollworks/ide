.PHONY: all
all:
	@echo "Targets:"
	@echo "  - build.debug"
	@echo "  - build.release"
	@echo "  - run.debug"
	@echo "  - run.debug.gdb"
	@echo "  - run.release"
	@echo "  - install.debug"
	@echo "  - install.release"
	@echo "  - ci.check"

.PHONY: build.debug
build.debug:
	@./autogen.sh
	@make -C __build__/debug/

.PHONY: build.release
build.release:
	@./autogen.sh -r
	@make -C __build__/release/

.PHONY: run.debug
run.debug: build.debug
	@./__build__/debug/sources/editor/trollworks-editor.exe

.PHONY: run.debug.gdb
run.debug.gdb: build.debug
	@gdb ./__build__/debug/sources/editor/trollworks-editor.exe

.PHONY: run.release
run.release: build.release
	@./__build__/release/sources/editor/trollworks-editor.exe

.PHONY: install.debug
install.debug:
	@./autogen.sh
	@make -C __build__/debug/ install

.PHONY: install.release
install.release:
	@./autogen.sh -r
	@make -C __build__/release/ install

.PHONY: ci.check
ci.check:
	@cppcheck --project=./__build__/debug/compile_commands.json -ivendor --check-level=exhaustive -q --error-exitcode=1
