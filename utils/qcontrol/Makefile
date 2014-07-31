#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qcontrol
PKG_VERSION:=0.5.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://www.hellion.org.uk/qcontrol/releases/$(PKG_VERSION)
PKG_MD5SUM:=5cc9e4c14bc2f804a8fed2b97f7553da

include $(INCLUDE_DIR)/package.mk

define Package/qcontrol
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+liblua +libpthread
  TITLE:=Hardware control utility for QNAP NAS devices
  URL:=http://www.hellion.org.uk/blog/
endef

define Package/qcontrol/description
Many NAS devices have LEDs, buttons, a fan and a watchdog that are connected
via a serial port or GPIOs and must be controlled from userspace. qcontrol was
originally written for the QNAP TS-109/TS-209 but was designed in such a way
that new device support can be easily added. 
endef

define Package/qcontrol/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/$(PKG_NAME).conf $(1)/etc/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/init.d/$(PKG_NAME)
endef

define Package/qcontrol/conffiles
/etc/$(PKG_NAME).conf
endef

$(eval $(call BuildPackage,qcontrol))
