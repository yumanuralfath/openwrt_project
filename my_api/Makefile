# OpenWrt Makefile for my_api package (no external dependencies)
# Place this in: package/my_package/my_api/Makefile

include $(TOPDIR)/rules.mk

PKG_NAME:=my_api
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/my_api
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Simple HTTP API Server
	# No external dependencies needed
endef

define Package/my_api/description
	A simple HTTP API server using standard sockets for OpenWrt
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/myapi \
		$(PKG_BUILD_DIR)/main.c
endef

define Package/my_api/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/myapi $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/myapi.init $(1)/etc/init.d/myapi
endef

$(eval $(call BuildPackage,my_api))
