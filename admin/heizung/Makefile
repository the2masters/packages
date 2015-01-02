#
# Copyright (C) 2014 Stefan Hellermann <stefan@the2masters.de>
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=heizung
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0+

include $(INCLUDE_DIR)/package.mk

define Package/heizung
  SECTION:=admin
  CATEGORY:=Administration
  TITLE:=Mein Heizungsausleseprogramm
  MAINTAINER:=Stefan Hellermann <stefan@openwrt.org>
  DEPENDS:=+zabbix-sender +kmod-usb-core +kmod-usb2 +kmod-usb-serial +kmod-usb-serial-ftdi
endef

define Package/heizung/description
Mein Heizungsausleseprogramm
endef


define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Wall -std=c99 \
		-o $(PKG_BUILD_DIR)/heizung $(PKG_BUILD_DIR)/main.c
endef


define Package/heizung/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/heizung $(1)/usr/sbin/heizung
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/heizung.init $(1)/etc/init.d/heizung
endef

$(eval $(call BuildPackage,heizung))
