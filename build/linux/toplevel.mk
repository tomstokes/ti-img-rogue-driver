########################################################################### ###
#@File
#@Copyright     Copyright (c) Imagination Technologies Ltd. All Rights Reserved
#@License       Dual MIT/GPLv2
#
# The contents of this file are subject to the MIT license as set out below.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Alternatively, the contents of this file may be used under the terms of
# the GNU General Public License Version 2 ("GPL") in which case the provisions
# of GPL are applicable instead of those above.
#
# If you wish to allow use of your version of this file only under the terms of
# GPL, and not to allow others to use your version of this file under the terms
# of the MIT license, indicate your decision by deleting the provisions above
# and replace them with the notice and other provisions required by GPL as set
# out in the file called "GPL-COPYING" included in this distribution. If you do
# not delete the provisions above, a recipient may use your version of this file
# under the terms of either the MIT license or GPL.
#
# This License is also included in this distribution in the file called
# "MIT-COPYING".
#
# EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
# PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
### ###########################################################################

# Define the default goal. This masks a previous definition of the default
# goal in config/core.mk, which must match this one
.PHONY: build
build: components firmware

MAKE_TOP      := build/linux
THIS_MAKEFILE := (top-level makefiles)

include $(MAKE_TOP)/defs.mk

ifeq ($(OUT),)
$(error Must specify output directory with OUT=)
endif

ifeq ($(TOP),)
$(error Must specify root of source tree with TOP=)
endif
$(call directory-must-exist,$(TOP))

# RELATIVE_OUT is relative only if it's under $(TOP)
RELATIVE_OUT       := $(patsubst $(TOP)/%,%,$(OUT))
CONFIG_MK          := $(RELATIVE_OUT)/config.mk
CONFIG_H           := $(RELATIVE_OUT)/config.h
CONFIG_KERNEL_MK   := $(RELATIVE_OUT)/config_kernel.mk
CONFIG_KERNEL_H	   := $(RELATIVE_OUT)/config_kernel.h

# Convert commas to spaces in $(D). This is so you can say "make
# D=config-changes,freeze-config" and have $(filter config-changes,$(D))
# still work.
override D := $(subst $(comma),$(space),$(D))

ifneq ($(INTERNAL_CLOBBER_ONLY),true)
# Create the out directory
#
$(shell mkdir -p $(OUT))

# If these generated files differ from any pre-existing ones,
# replace them, causing affected parts of the driver to rebuild.
#
_want_config_diff := $(filter config-changes,$(D))
_freeze_config := $(strip $(filter freeze-config,$(D)))
_updated_config_files := $(shell \
    $(if $(_want_config_diff),rm -f $(OUT)/config.diff;,) \
	for file in $(CONFIG_MK) $(CONFIG_H) \
				$(CONFIG_KERNEL_MK) $(CONFIG_KERNEL_H); do \
		diff -U 0 $$file $$file.new \
			>>$(if $(_want_config_diff),$(OUT)/config.diff,/dev/null) 2>/dev/null \
		&& rm -f $$file.new \
		|| echo $$file; \
	done)

ifneq ($(_want_config_diff),)
# We send the diff to stderr so it isn't captured by $(shell)
$(shell [ -s $(OUT)/config.diff ] && echo >&2 "Configuration changed in $(RELATIVE_OUT):" && cat >&2 $(OUT)/config.diff)
endif

ifneq ($(_freeze_config),)
$(if $(_updated_config_files),$(error Configuration change in $(RELATIVE_OUT) prevented by D=freeze-config),)
endif

# Update the config, if changed
$(foreach _f,$(_updated_config_files), \
	$(shell mv -f $(_f).new $(_f) >/dev/null 2>/dev/null))

endif # INTERNAL_CLOBBER_ONLY

MAKEFLAGS := -Rr --no-print-directory

ifneq ($(INTERNAL_CLOBBER_ONLY),true)

# This is so you can say "find $(TOP) -name Linux.mk > /tmp/something; export
# ALL_MAKEFILES=/tmp/something; make" and avoid having to run find. This is
# handy if your source tree is mounted over NFS or something
override ALL_MAKEFILES := $(call relative-to-top,$(if $(strip $(ALL_MAKEFILES)),$(shell cat $(ALL_MAKEFILES)),$(shell find $(TOP) -type f -name Linux.mk -print -o -type d -name '.*' -prune)))
ifeq ($(strip $(ALL_MAKEFILES)),)
$(info ** Unable to find any Linux.mk files under $$(TOP). This could mean that)
$(info ** there are no makefiles, or that ALL_MAKEFILES is set in the environment)
$(info ** and points to a nonexistent or empty file.)
$(error No makefiles)
endif

else # clobber-only
ALL_MAKEFILES :=
endif

unexport ALL_MAKEFILES

REMAINING_MAKEFILES := $(ALL_MAKEFILES)
ALL_MODULES :=
INTERNAL_INCLUDED_ALL_MAKEFILES :=

ALL_LDFLAGS :=

# Please do not change the format of the following lines
-include $(CONFIG_MK)
-include $(CONFIG_KERNEL_MK)
# OK to change now

define is-host-os
$(if $(HOST_OS),$(if $(filter $(1),$(HOST_OS)),true),$(error HOST_OS not set))
endef

define is-not-host-os
$(if $(HOST_OS),$(if $(filter-out $(1),$(HOST_OS)),true),$(error HOST_OS not set))
endef

define is-target-os
$(if $(TARGET_OS),$(if $(filter $(1),$(TARGET_OS)),true),$(error TARGET_OS not set))
endef

define is-not-target-os
$(if $(TARGET_OS),$(if $(filter-out $(1),$(TARGET_OS)),true),$(error TARGET_OS not set))
endef

# If we haven't set host/target archs, set some sensible defaults now.
# This allows things like prune.sh to work
ifeq ($(HOST_PRIMARY_ARCH),)
ifneq ($(FORCE_ARCH),)
HOST_PRIMARY_ARCH := host_x86_64
HOST_32BIT_ARCH := host_i386

# We set HOST_ALL_ARCH this way, as HOST_32BIT_ARCH may be overridden on the
# command line.
ifeq ($(HOST_PRIMARY_ARCH),$(HOST_32BIT_ARCH))
HOST_ALL_ARCH := $(HOST_PRIMARY_ARCH)
else
HOST_ALL_ARCH := $(HOST_PRIMARY_ARCH) $(HOST_32BIT_ARCH)
endif

_ALL_NEUTRINO_ARCHS := $(wildcard $(MAKE_TOP)/moduledefs/target_nto*.mk)

ifeq ($(SUPPORT_NEUTRINO_PLATFORM),)
 _ALL_ARCHS := \
  $(filter-out %target_neutral.mk $(_ALL_NEUTRINO_ARCHS),\
   $(wildcard $(MAKE_TOP)/moduledefs/target_*.mk))
else
 _ALL_ARCHS := $(_ALL_NEUTRINO_ARCHS)
endif

TARGET_PRIMARY_ARCH := \
 $(patsubst $(MAKE_TOP)/moduledefs/%.mk,%,$(word 1, $(_ALL_ARCHS)))

TARGET_ALL_ARCH := $(TARGET_PRIMARY_ARCH)
endif
endif

# Output directory for configuration, object code,
# final programs/libraries, and install/rc scripts.
HOST_OUT             := $(RELATIVE_OUT)/$(HOST_PRIMARY_ARCH)
HOST_32BIT_OUT       := $(RELATIVE_OUT)/$(HOST_32BIT_ARCH)
TARGET_OUT           := $(RELATIVE_OUT)/$(TARGET_PRIMARY_ARCH)
TARGET_PRIMARY_OUT   := $(RELATIVE_OUT)/$(TARGET_PRIMARY_ARCH)
TARGET_NEUTRAL_OUT   := $(RELATIVE_OUT)/target_neutral
BRIDGE_SOURCE_ROOT   := $(call if-exists,$(TOP)/generated/$(PVR_ARCH),$(TARGET_NEUTRAL_OUT)/intermediates)
GENERATED_CODE_OUT   := $(TARGET_NEUTRAL_OUT)/intermediates
DOCS_OUT             := $(RELATIVE_OUT)/doc

#
# neutrino/subst_makefiles.mk must be included after Output directories have been defined,
# because it overrides BRIDGE_SOURCE_ROOT of bridges to be built. If we include this makefile
# earlier, the value of BRIDGE_SOURCE_ROOT set in neutrino/subst_makefiles.mk will be overwritten.
ifeq ($(SUPPORT_NEUTRINO_PLATFORM),1)
include $(MAKE_TOP)/common/neutrino/subst_makefiles.mk
# neutrino/subst_makefiles.mk overrides ALL_MAKEFILES.
# Set REMAINING_MAKEFILES to the new value of ALL_MAKEFILES
REMAINING_MAKEFILES := $(ALL_MAKEFILES)
endif

# Mark subdirectories of $(OUT) as secondary, and provide rules to create
# them.
OUT_SUBDIRS := $(addprefix $(RELATIVE_OUT)/,$(TARGET_ALL_ARCH)) \
 $(TARGET_NEUTRAL_OUT) $(DOCS_OUT) $(if $(HOST_PRIMARY_ARCH),$(sort $(HOST_OUT) $(HOST_32BIT_OUT)))
.SECONDARY: $(OUT_SUBDIRS)
$(OUT_SUBDIRS):
	$(make-directory)

ifeq ($(PVR_CREATE_TARGET_PRIMARY_OUT_LINK),1)
TARGET_PRIMARY_OUT_LINK := $(RELATIVE_OUT)/target_primary
.SECONDARY: $(TARGET_PRIMARY_OUT_LINK)
$(TARGET_PRIMARY_OUT_LINK):
	$(LN) $(TARGET_PRIMARY_ARCH) $@

$(TARGET_PRIMARY_OUT): | $(TARGET_PRIMARY_OUT_LINK)
endif

ifneq ($(INTERNAL_CLOBBER_ONLY),true)
-include $(MAKE_TOP)/pvrversion.mk
-include $(MAKE_TOP)/llvm.mk
ifeq ($(SERVICES_SC),1)
 -include $(MAKE_TOP)/common/bridges_sc.mk
else
 -include $(MAKE_TOP)/common/bridges.mk
endif
endif

ifeq ($(INTERNAL_CLOBBER_ONLY)$(SUPPORT_ANDROID_PLATFORM)$(SUPPORT_NEUTRINO_PLATFORM)$(SUPPORT_INTEGRITY_PLATFORM),)
 # doing a Linux build.  We need to worry about sysroots.

 # The directories containing linux window system related components
 LWS_DIR           := $(TOP)/lws
 LWS_TARBALL_DIR   ?= $(TOP)/external/lws
 LWS_PATCH_DIR     := $(LWS_DIR)/dist/patches
 LWS_GIT_PATCH_DIR := $(LWS_DIR)/patches

 ifneq ($(SUPPORT_BUILD_LWS),)
  -include $(MAKE_TOP)/lwsconf.mk

 else ifneq ($(SYSROOT),)
  LWS_PREFIX ?= /usr
  XORG_CONFDIR ?= /etc/X11

  override SYS_CFLAGS      += --sysroot=${SYSROOT}
  override SYS_CXXFLAGS    += --sysroot=${SYSROOT}
  override SYS_LDFLAGS     += --sysroot=${SYSROOT}

  PKG_CONFIG_SYSROOT_DIR := ${SYSROOT}

  ifneq ($(SYSROOT),/)
   # Override PKG_CONFIG_PATH to prevent additional host paths from being
   # searched
   PKG_CONFIG_PATH :=
  endif

 else ifneq ($(CROSS_COMPILE),)
  $(info WARNING: You are not specifying how to find dependent libraries, e.g., by specifying SYSROOT.)
  $(info          The build may fail.)
 endif
endif

# This has to come after including lwsconf.mk since it relies on LWS_PREFIX
# to construct the path to wayland-scanner
include $(MAKE_TOP)/commands.mk

BUILD_CACHES=$(sort $(LWS_BUILD_CACHES))
ifneq ($(BUILD_CACHES),)
.SECONDARY: $(BUILD_CACHES)
$(BUILD_CACHES):
	$(make-directory)
endif

# We don't need to include this if we're just doing a clean or a clobber
#
ifneq ($(INTERNAL_CLOBBER_ONLY),true)
include $(MAKE_TOP)/buildvars.mk
endif

HOST_INTERMEDIATES := $(HOST_OUT)/intermediates
TARGET_INTERMEDIATES := $(TARGET_OUT)/intermediates

ifneq ($(KERNEL_COMPONENTS),)
build: kbuild
endif

# The 'bridges' target is used to make all the modules that are of the bridge
# type. One of its main uses is to pregenerate the bridge files for inclusion
# into the DDK source packages. Since there is no need and no facility to
# generate the bridge files when the pregenerated files exist, make the target
# a no-op in this case.
.PHONY: bridges
ifeq ($(BRIDGE_SOURCE_ROOT),$(TOP)/generated/$(PVR_ARCH))
bridges:
else
bridges: $(BRIDGES)
endif

# Include each Linux.mk, then include modules.mk to save some information
# about each module
include $(foreach _Linux.mk,$(ALL_MAKEFILES),$(MAKE_TOP)/this_makefile.mk $(_Linux.mk) $(MAKE_TOP)/modules.mk)

ifeq ($(strip $(REMAINING_MAKEFILES)),)
INTERNAL_INCLUDED_ALL_MAKEFILES := true
else
$(error Impossible: $(words $(REMAINING_MAKEFILES)) makefiles were mysteriously ignored when reading $$(ALL_MAKEFILES))
endif

# For the special "all_modules" target, filter out any kernel_module modules
# These are explicitly non-buildable in isolation.
ALL_NON_KERNEL_MODULES := \
 $(foreach _m,$(ALL_MODULES),\
  $(if $(filter-out kernel_module,$($(_m)_type)),$(_m)))

# Compute the isystem paths passed in via SYS_INCLUDES. We'll use this in
# the module target_xxx makefiles to filter duplicate -isystem and -I flags,
# to ensure the module can always override the include precedence. (Also
# calculate any 'residual' non-include flags, as we need to put them back.)
SYS_INCLUDES_ISYSTEM := \
 $(subst -isystem,,$(filter -isystem%,$(subst -isystem ,-isystem,$(SYS_INCLUDES))))
SYS_INCLUDES_RESIDUAL := \
 $(strip $(filter-out -isystem%,$(subst -isystem ,-isystem,$(SYS_INCLUDES))))

# At this point, all Linux.mks have been included. Now generate rules to build
# each module: for each module in $(ALL_MODULES), set per-makefile variables
ALL_BAD_MODULES :=
$(foreach _m,$(ALL_MODULES),$(eval $(call process-module,$(_m))))

# Filter out any modules that were missing their module 'type' makefile. These
# modules will break the packaging process and can't be built anyway.
ALL_MODULES := $(filter-out $(ALL_BAD_MODULES),$(ALL_MODULES))

.PHONY: kbuild install
kbuild install:

ifneq ($(INTERNAL_CLOBBER_ONLY),true)
 -include $(MAKE_TOP)/scripts.mk
 -include $(MAKE_TOP)/kbuild/kbuild.mk
endif
# We won't depend on 'build' here so that people can build subsets of
# components and still have the install script attempt to install the
# subset.
install:
	@if [ ! -d "$(DISCIMAGE)" -a -z "$(INSTALL_TARGET)" ]; then \
		echo; \
		echo "** DISCIMAGE was not set or does not point to a valid directory."; \
		echo "** Either use INSTALL_TARGET or set DISCIMAGE."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@if [ ! -f $(RELATIVE_OUT)/install.sh ]; then \
		echo; \
		echo "** install.sh not found in $(RELATIVE_OUT)."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@cd $(RELATIVE_OUT) && ./install.sh

installfw: install_script_fw
	@if [ ! -f $(RELATIVE_OUT)/install.sh ]; then \
		echo; \
		echo "** install.sh not found in $(RELATIVE_OUT)."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@cd $(RELATIVE_OUT) && ./install.sh --fw

installcomponents: install_script
	@if [ ! -f $(RELATIVE_OUT)/install.sh ]; then \
		echo; \
		echo "** install.sh not found in $(RELATIVE_OUT)."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@cd $(RELATIVE_OUT) && ./install.sh --um

installkm: install_script_km
	@if [ ! -d "$(DISCIMAGE)" -a -z "$(INSTALL_TARGET)" ]; then \
		echo; \
		echo "** DISCIMAGE was not set or does not point to a valid directory."; \
		echo "** Either use INSTALL_TARGET or set DISCIMAGE."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@if [ ! -f $(RELATIVE_OUT)/install.sh ]; then \
		echo; \
		echo "** install.sh not found in $(RELATIVE_OUT)."; \
		echo "** Cannot continue with install."; \
		echo; \
		exit 1; \
	fi
	@cd $(RELATIVE_OUT) && ./install.sh --km

.PHONY: uninstall
uninstall: install_script install_script_fw install_script_km
	@if [ ! -d "$(DISCIMAGE)" -a -z "$(INSTALL_TARGET)" ]; then \
		echo; \
		echo "** DISCIMAGE was not set or does not point to a valid directory."; \
		echo "** Either use INSTALL_TARGET or set DISCIMAGE."; \
		echo "** Cannot continue with uninstall."; \
		echo; \
		exit 1; \
	fi
	@if [ ! -f $(RELATIVE_OUT)/install.sh ]; then \
		echo; \
		echo "** install.sh not found in $(RELATIVE_OUT)."; \
		echo "** Cannot continue with uninstall."; \
		echo; \
		exit 1; \
	fi
	@cd $(RELATIVE_OUT) && ./install.sh -u

# You can say 'make all_modules' to attempt to make everything, or 'make
# components' to only make the things which are listed (in the per-build
# makefiles) as components of the build.
.PHONY: all_modules all_docs components firmware
all_modules: $(ALL_NON_KERNEL_MODULES) kbuild
all_docs: ;
components: $(COMPONENTS)
firmware: $(FW_COMPONENTS)

# Cleaning
.PHONY: clean clobber
clean: MODULE_DIRS_TO_REMOVE := $(OUT_SUBDIRS)
clean:
	$(if $(TARGET_PRIMARY_OUT_LINK),$(RM) $(TARGET_PRIMARY_OUT_LINK))
	$(clean-dirs)
clobber: MODULE_DIRS_TO_REMOVE := $(OUT)
clobber:
	$(clean-dirs)

# Saying 'make clean-MODULE' removes the intermediates for MODULE.
# clobber-MODULE deletes the output files as well
clean-%:
	$(if $(V),,@echo "  RM      " $(call relative-to-top,$(INTERNAL_CLEAN_TARGETS_FOR_$*)))
	$(RM) -rf $(INTERNAL_CLEAN_TARGETS_FOR_$*)
clobber-%:
	$(if $(V),,@echo "  RM      " $(call relative-to-top,$(INTERNAL_CLOBBER_TARGETS_FOR_$*)))
	$(RM) -rf $(INTERNAL_CLOBBER_TARGETS_FOR_$*)

include $(MAKE_TOP)/bits.mk

# D=nobuild stops the build before any recipes are run. This line should
# come at the end of this makefile.
$(if $(filter nobuild,$(D)),$(error D=nobuild given),)
