.DEFAULT_GOAL := usage

# terminal colours
RED     = \033[0;31m
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
NC      = \033[0m

define ANNOUNCE
echo "$(2)$(1)${NC}"
endef

tmp:
	mkdir -p tmp

.PHONY: brew
brew:
	brew bundle
	@echo "need to get MacOS silicon USB driver from https://github.com/WCHSoftGroup/ch34xser_macos"

tmp/CH34xVCPDriver.pkg:
	wget \
		https://github.com/WCHSoftGroup/ch34xser_macos/raw/refs/heads/main/CH34xVCPDriver.pkg \
		--output-document tmp/CH34xVCPDriver.pkg 

tmp/CH341SER_MAC.ZIP:
	wget \
		"https://api.wch.cn/api/official/website/common/downloadFile?fileName=CH341SER_MAC.ZIP" \
		--output-document tmp/CH341SER_MAC.ZIP

.PHONY: setup
setup: brew tmp tmp/CH34xVCPDriver.pkg tmp/CH341SER_MAC.ZIP
	@echo "to install USB drivers for Arduino Nano"
	@echo "\t${GREEN}open tmp/CH34xVCPDriver.pkg${NC}"
	@echo "or"
	@echo "\t${GREEN}open tmp/CH341SER_MAC.ZIP${NC}"

.PHONY: clean
clean:
	rm -rf tmp

.PHONY: usage
usage:
	@echo
	@echo "Hi ${GREEN}${USER}!${NC} Welcome to ${RED}${CURRENT_DIR}${NC}"
	@echo
	@echo "${YELLOW}make${NC}                         this handy menu"
	@echo
	@echo "${YELLOW}make setup${NC}                   setup computer (assuming Mac and Brew)"
	@echo
	@echo "${YELLOW}make clean${NC}                   remove tmp directory"
	@echo
