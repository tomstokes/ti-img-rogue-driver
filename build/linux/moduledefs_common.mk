########################################################################### ###
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

MODULE_OUT := $(RELATIVE_OUT)/$(MODULE_ARCH)
MODULE_INTERMEDIATES_DIR := $(MODULE_OUT)/intermediates/$(THIS_MODULE)

MODULE_TARGETS :=
MODULE_HOST_BUILD :=
MODULE_CLEAN_TARGETS :=
MODULE_CLOBBER_TARGETS :=

MODULE_CFLAGS :=
MODULE_CXXFLAGS :=
MODULE_LDFLAGS :=
MODULE_BISON_FLAGS :=
MODULE_FLEX_FLAGS :=
MODULE_FLEXXX_FLAGS :=

MODULE_ARCH_TAG := $(patsubst i%86,i686,$(subst host_,,$(subst target_,,$(MODULE_ARCH))))
MODULE_ARCH_BITNESS :=

# Only allow cflags that do not affect code generation. This is to ensure
# proper binary compatibility when LTO (Link-Time Optimization) is enabled.
# We make exceptions for the below flags which will all fail linkage in
# non-LTO mode if incorrectly specified.
#
# NOTE: Only used by static_library and objects right now. Other module
# types should not be affected by complex code generation flags w/ LTO.
# Set MODULE_CHECK_CFLAGS in the module makefile to enable this check.
MODULE_CHECK_CFLAGS :=
MODULE_ALLOWED_CFLAGS := -W% -D% -std=% -frtti -fPIC -fPIE -pie -m32 -fvisibility=hidden -fexceptions -fgnu89-inline

# -L flags for library search dirs: these are relative to $(TOP), unless
# they're absolute paths
MODULE_LIBRARY_DIR_FLAGS := $(foreach _path,$($(THIS_MODULE)_libpaths),$(if $(filter /%,$(_path)),-L$(call relative-to-top,$(_path)),-L$(_path)))
# -L options to find system libraries (may be arch-specific)
MODULE_SYSTEM_LIBRARY_DIR_FLAGS :=
# -I flags for header search dirs (same rules as for -L)
MODULE_INCLUDE_FLAGS := $(foreach _path,$($(THIS_MODULE)_includes),$(if $(filter /%,$(_path)),-I$(call relative-to-top,$(_path)),-I$(_path)))
# Pattern substitution in form old:new to be done to link command line
MODULE_LIBRARY_FLAGS_SUBST :=

# If the build provides some external khronos include flags, and the module
# hasn't explicitly opted out of path substitution, prepend the system path
# to the DDK khronos header include path. This causes the platform headers
# to override the DDK versions. This is the default behaviour for Android.
ifneq ($(filter-out host_%,$(MODULE_ARCH)),)
ifneq ($(SYS_KHRONOS_INCLUDES),)
ifneq ($($(THIS_MODULE)_force_internal_khronos_headers),1)
MODULE_INCLUDE_FLAGS := $(patsubst -Iinclude/khronos,$(SYS_KHRONOS_INCLUDES) -isystem include/khronos,$(MODULE_INCLUDE_FLAGS))
endif
endif
endif

# These define the rules for finding source files.
#
# - If a name begins with a slash, we strip $(TOP) off the front if it
#   begins with $(TOP). This is so that we don't get really long error
#   messages from the compiler if the source tree is in a deeply nested
#   directory, but we still do get absolute paths if you say "make
#   OUT=/tmp/somewhere"
#
# - Otherwise, if a name contains a slash and begins with $(OUT), we leave
#   it as it is. This is so you can say "module_src :=
#   $(TARGET_INTERMEDIATES)/something/generated.c"
#
# - Otherwise, we assume it's a path referring to somewhere under the
#   directory containing Linux.mk, and add $(THIS_DIR) to it
#
_SOURCES_WITHOUT_SLASH := \
 $(strip $(foreach _s,$($(THIS_MODULE)_src),$(if $(findstring /,$(_s)),,$(_s))))
_SOURCES_WITH_SLASH := \
 $(strip $(foreach _s,$($(THIS_MODULE)_src),$(if $(findstring /,$(_s)),$(_s),)))
MODULE_SOURCES := $(addprefix $(THIS_DIR)/,$(_SOURCES_WITHOUT_SLASH))
MODULE_SOURCES += $(call relative-to-top,$(abspath $(filter /%,$(_SOURCES_WITH_SLASH))))

_RELATIVE_SOURCES_WITH_SLASH := \
 $(filter-out /%,$(_SOURCES_WITH_SLASH))
_OUTDIR_RELATIVE_SOURCES_WITH_SLASH := \
 $(filter $(RELATIVE_OUT)/%,$(_RELATIVE_SOURCES_WITH_SLASH))
_THISDIR_RELATIVE_SOURCES_WITH_SLASH := \
 $(filter-out $(RELATIVE_OUT)/%,$(_RELATIVE_SOURCES_WITH_SLASH))
MODULE_SOURCES += $(call relative-to-top,$(abspath $(_OUTDIR_RELATIVE_SOURCES_WITH_SLASH)))
MODULE_SOURCES += $(call relative-to-top,$(abspath $(addprefix $(THIS_DIR)/,$(_THISDIR_RELATIVE_SOURCES_WITH_SLASH))))

# Add generated sources
MODULE_SOURCES += $(call relative-to-top,$(abspath $(addprefix $(MODULE_OUT)/,$($(THIS_MODULE)_src_relative))))

# We want to do this only for pure Android, in which case only
# SUPPORT_ANDROID_PLATFORM will be set to 1.
ifeq ($(SUPPORT_ANDROID_PLATFORM)$(SUPPORT_ARC_PLATFORM),1)
 define set-flags-from-package
  ifeq ($(1),libdrm)
   ifeq ($(PVR_ANDROID_OLD_LIBDRM_HEADER_PATH),1)
    $(THIS_MODULE)_includes += \
     $(TARGET_ROOT)/product/$(TARGET_DEVICE)/obj/include \
     $(TARGET_ROOT)/product/$(TARGET_DEVICE)/obj/include/libdrm
   endif
  else ifeq ($(1),sync)
   # Nothing to add in this case
  else
   $$(warning Unknown package for '$(THIS_MODULE)': $(1))
   $$(error Missing mapping between package and compiler flags)
  endif
 endef

 $(foreach _package,$($(THIS_MODULE)_packages),\
  $(eval $(call set-flags-from-package,$(_package))))
else ifeq ($(SUPPORT_NEUTRINO_PLATFORM),)
 # pkg-config integration
 # We don't support arbitrary CFLAGS yet (just includes)
 $(foreach _package,$($(THIS_MODULE)_packages),\
  $(eval MODULE_INCLUDE_FLAGS     += `$(PKG_CONFIG) --cflags-only-I $(_package)`)\
  $(eval MODULE_LIBRARY_DIR_FLAGS += `$(PKG_CONFIG) --libs-only-L $(_package)`))
endif
