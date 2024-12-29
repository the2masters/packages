#
# Copyright (C) 2024 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libvirt
PKG_VERSION:=10.10.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://download.libvirt.org
PKG_HASH:=e1bd7bd31b7c0d0ae073dec050bb5b0232b3e4adebdc58ea82fe8b366c765796
PKG_MAINTAINER:=Stefan Hellermann <stefan@the2masters.de>
PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=COPYING

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libvirt
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Library providing a simple virtualization API
  URL:=https://libvirt.org
  DEPENDS:= +glib2 +kmod-tun +libacl +libbsd +libgnutls +libjson-c +libtirpc +libxml2 +parted
endef

define Package/libvirt/description
	TODO
endef

define Package/libvirt/conffiles
/etc/libvirt/
endef


MESON_ARGS += -Dauto_features=disabled \
	-Dsystem=true \
	-Djson_c=enabled \
	-Ddriver_libvirtd=enabled \
	-Ddriver_network=enabled \
	-Ddriver_remote=enabled \
	-Ddriver_qemu=enabled \
	-Dstorage_dir=enabled \
	-Dstorage_disk=enabled \
	-Dfirewall_backend_priority=none \
	-Dqemu_user=root -Dqemu_group=root


define Package/libvirt/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/{libvirtd,libvirt-guests,virtlogd,virtlockd} $(1)/etc/init.d/

	$(INSTALL_DIR) $(1)/usr/share/qemu/firmware
	$(CP) files/40-edk2-x86_64-secure-enrolled.json $(1)/usr/share/qemu/firmware/
	$(CP) files/50-edk2-aarch64-qcow2.json $(1)/usr/share/qemu/firmware/
	$(CP) files/51-edk2-ovmf-2m-raw-x64-nosb.json $(1)/usr/share/qemu/firmware/

	$(INSTALL_DIR) $(1)/usr/share/OVMF
	$(CP) files/OVMF_{CODE,VARS}.fd $(1)/usr/share/OVMF/
	$(CP) files/OVMF_{CODE,VARS}_4M.ms.fd $(1)/usr/share/OVMF/

	$(INSTALL_DIR) $(1)/usr/share/edk2/aarch64
	$(CP) files/QEMU_EFI-silent-pflash.qcow2 $(1)/usr/share/edk2/aarch64
	$(CP) files/vars-template-pflash.qcow2 $(1)/usr/share/edk2/aarch64

	$(INSTALL_DIR) $(1)/etc/libvirt
	$(INSTALL_CONF) $(PKG_INSTALL_DIR)/etc/libvirt/{libvirt*,qemu{,-lockd}.conf,virt*}  $(1)/etc/libvirt/

	$(INSTALL_DIR) $(1)/etc/libvirt/qemu/autostart
	$(INSTALL_DIR) $(1)/etc/libvirt/qemu/storage/autostart
	$(INSTALL_DIR) $(1)/etc/libvirt/qemu/networks/autostart
	$(INSTALL_CONF) files/br-lan.xml $(1)/etc/libvirt/qemu/networks
	$(LN) ../br-lan.xml $(1)/etc/libvirt/qemu/networks/autostart

	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/* $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/libexec
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/* $(1)/usr/libexec/

	$(INSTALL_DIR) $(1)/usr/lib/libvirt
	$(CP) $(PKG_INSTALL_DIR)/usr/lib64/libvirt/* $(1)/usr/lib/libvirt/

	$(INSTALL_DIR) $(1)/usr/share/libvirt/cpu_map
	$(CP) $(PKG_INSTALL_DIR)/usr/share/libvirt/cpu_map/* $(1)/usr/share/libvirt/cpu_map/

	$(INSTALL_DIR) $(1)/usr/share/libvirt/schemas
	$(CP) $(PKG_INSTALL_DIR)/usr/share/libvirt/schemas/* $(1)/usr/share/libvirt/schemas/
endef

$(eval $(call BuildPackage,libvirt))
