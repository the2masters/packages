#
# Copyright (C) 2024 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_SOURCE_NAME:=usbredir
PKG_NAME:=libusbredirparser
PKG_VERSION:=0.13.0
PKG_RELEASE:=1

PKG_SOURCE:=${PKG_SOURCE_NAME}-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://spice-space.org/download/${PKG_SOURCE_NAME}/
PKG_HASH:=4ba6faa02c0ae6deeb4c53883d66ab54b3a5899bead42ce4ded9568b9a7dc46e

PKG_MAINTAINER:=Stefan Hellermann <stefan@the2masters.de>
PKG_LICENSE:=LGPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libusbredirparser
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A library containing the parser for the usbredir protocol
  URL:=https://libvirt.org
  DEPENDS:= +libusb-1.0
endef

MESON_ARGS += -Dauto_features=disabled \
	-Dprefix=/usr \

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libusbredirparser.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/* $(1)/usr/lib/pkgconfig
endef

define Package/libusbredirparser/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libusbredirparser.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libusbredirparser))
