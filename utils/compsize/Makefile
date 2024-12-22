#
# Copyright (C) 2024 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=compsize
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/kilobyte/compsize.git
PKG_SOURCE_DATE:=2024-12-22
PKG_SOURCE_VERSION:=d79eacf77abe3b799387bb8a4e07a18f1f1031e8

PKG_MAINTAINER:=Stefan Hellermann <stefan@the2masters.de>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE
PKG_BUILD_DEPENDS:=btrfs-progs

PKG_INSTALL:=1

TARGET_CFLAGS += -D_GNU_SOURCE

include $(INCLUDE_DIR)/package.mk

define Package/compsize
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Show btrfs compression type/ratio
  URL:=https://github.com/kilobyte/compsize
endef

define Package/compsize/description
  compsize takes a list of files (given as arguments) on a btrfs filesystem and
  measures used compression types and effective compression ratio.
endef

define Package/compsize/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/compsize $(1)/usr/bin/
endef

$(eval $(call BuildPackage,compsize))
