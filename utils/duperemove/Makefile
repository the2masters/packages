#
# Copyright (C) 2024 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=duperemove
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/markfasheh/duperemove.git
PKG_SOURCE_DATE:=2024-12-22
PKG_SOURCE_VERSION:=c389d3d5309ed5641aae8cb5d7a255019396bf86

PKG_MAINTAINER:=Stefan Hellermann <stefan@the2masters.de>
PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=LICENSE
PKG_BUILD_DEPENDS:=libbsd

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/duperemove
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Tools for deduping file systems
  URL:=https://github.com/markfasheh/duperemove
  DEPENDS:=+glib2 +libsqlite3 +libblkid +libmount +libuuid +libxxhash
endef

define Package/duperemove/description
  Duperemove is a simple tool for finding duplicated extents and submitting them
  for deduplication. When given a list of files it will hash their contents on
  an extent by extent basis and compare those hashes to each other, finding and
  categorizing extents that match each other. Optionally, a per-block hash can
  be applied for further duplication lookup. When given the -d option,
  duperemove will submit those extents for deduplication using the Linux kernel
  FIDEDUPRANGE ioctl.

  Duperemove can store the hashes it computes in a 'hashfile'. If given an
  existing hashfile, duperemove will only compute hashes for those files which
  have changed since the last run. Thus you can run duperemove repeatedly on
  your data as it changes, without having to re-checksum unchanged data.

  Duperemove can also take input from the fdupes program.
endef

define Package/duperemove/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/local/bin/duperemove $(1)/usr/bin/
endef

$(eval $(call BuildPackage,duperemove))
