.PHONY: clean All

All:
	@echo "----------Building project:[ Blocks - Debug ]----------"
	@"$(MAKE)" -f  "Blocks.mk"
clean:
	@echo "----------Cleaning project:[ Blocks - Debug ]----------"
	@"$(MAKE)" -f  "Blocks.mk" clean
