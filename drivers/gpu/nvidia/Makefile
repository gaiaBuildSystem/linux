# SPDX-License-Identifier: GPL-2.0
NVROOT := $(srctree)/drivers/gpu/nvidia

NVIDIA_IFACE_SOURCES := $(shell grep -h 'NVIDIA_SOURCES *+=' $(NVROOT)/nvidia/nvidia-sources.Kbuild | sed 's/.*+= //')
NVIDIA_CORE_SOURCES  := $(shell grep -h 'SRCS *+=' $(NVROOT)/nvidia/srcs.mk | sed 's/.*+= //;s|^|nvidia/|')
NVIDIA_SOURCES := $(NVIDIA_IFACE_SOURCES) $(NVIDIA_CORE_SOURCES)
obj-m += nvidia.o
nvidia-y := $(patsubst %.c,%.o,$(NVIDIA_SOURCES))
# defines pNVRM_ID (GENERATE_NVIDSTRING in the original build)
nvidia-y += nvidia/nvid-string.o

NVIDIA_DRM_SOURCES := $(shell grep -h 'NVIDIA_DRM_SOURCES *+=' $(NVROOT)/nvidia-drm/nvidia-drm-sources.mk | sed 's/.*+= //')
# added in the original nvidia-drm.Kbuild, not in nvidia-drm-sources.mk;
# carries the module's MODULE_LICENSE()
NVIDIA_DRM_SOURCES += nvidia-drm/nvidia-drm-linux.c
obj-m += nvidia-drm.o
nvidia-drm-y := $(patsubst %.c,%.o,$(NVIDIA_DRM_SOURCES))

ifdef CONFIG_NVIDIA_MODESET
# srcs.mk paths are relative to nvidia-modeset/: "../common/..." -> "common/...",
# "src/..." -> "nvidia-modeset/src/..."
nv_modeset_remap = sed 's/.*+= //; s|^\.\./||' | sed '/^common\//!s|^|nvidia-modeset/|'
NVIDIA_MODESET_SOURCES := nvidia-modeset/nvidia-modeset-linux.c nvidia-modeset/nv-kthread-q.c
NVIDIA_MODESET_SOURCES += $(shell grep -h 'SRCS *+=' $(NVROOT)/nvidia-modeset/srcs.mk | $(nv_modeset_remap))
NVIDIA_MODESET_CXX_SOURCES := $(shell grep -h 'SRCS_CXX *+=' $(NVROOT)/nvidia-modeset/srcs.mk | $(nv_modeset_remap))
obj-m += nvidia-modeset.o
nvidia-modeset-y := $(patsubst %.c,%.o,$(NVIDIA_MODESET_SOURCES))
nvidia-modeset-y += $(patsubst %.cpp,%.o,$(NVIDIA_MODESET_CXX_SOURCES))
# defines pNV_KMS_ID (GENERATE_NVIDSTRING in the original build)
nvidia-modeset-y += nvidia-modeset/src/nvid-string.o
# xz-compressed shader blobs embedded via ld --format=binary
# (COMPRESS_SHADERS + READ_ONLY_OBJECT_FROM_FILE_RULE in the original build)
nvidia-modeset-y += nvidia-modeset/turing_shaders.xz.o
nvidia-modeset-y += nvidia-modeset/ampere_shaders.xz.o

# this kbuild predates upstream C++ support; force .cpp through $(CC) with the
# C++ flags nvdisplay's stage-1 build uses (nvdisplay/src/nvidia-modeset/Makefile)
quiet_cmd_cc_o_cpp = C++    $@
      cmd_cc_o_cpp   = $(CC) -x c++ -std=gnu++11 -fno-operator-names -fno-rtti -fno-exceptions -fcheck-new $(c_flags) -c -o $@ $<
$(obj)/%.o: $(src)/%.cpp FORCE
	$(call if_changed_dep,cc_o_cpp)

# Embed the xz-compressed shader blobs; ld --format=binary names the
# symbols _binary_<file>_start/_end/_size (dots become underscores),
# matching the references in nvidia-modeset.
quiet_cmd_xz_shaders  = XZ     $@
      cmd_xz_shaders  = xz -ce -C none < $< > $@
quiet_cmd_bin_obj     = BINOBJ $@
      # cd must stay in a subshell: if_changed appends a .cmd write to the
      # same shell line, which needs the original cwd; the input basename
      # (notdir) determines the _binary_* symbol names
      cmd_bin_obj     = (cd $(dir $<) && $(LD) -r -z noexecstack --format=binary $(notdir $<) -o $(abspath $@)) && \
                        $(OBJCOPY) --rename-section .data=.rodata,contents,alloc,load,data,readonly $@
$(obj)/nvidia-modeset/%_shaders.xz: $(NVROOT)/nvidia-modeset/src/shaders/g_%_shaders FORCE
	$(call if_changed,xz_shaders)
$(obj)/nvidia-modeset/%_shaders.xz.o: $(obj)/nvidia-modeset/%_shaders.xz FORCE
	$(call if_changed,bin_obj)
clean-files += nvidia-modeset/turing_shaders.xz nvidia-modeset/ampere_shaders.xz
endif

ccflags-y += -include $(NVROOT)/common/sdk/nvidia/inc/cpuopsys.h
ccflags-y += -I$(NVROOT)
ccflags-y += -I$(NVROOT)/nvidia
ccflags-y += -I$(NVROOT)/nvidia/interface
ccflags-y += -I$(NVROOT)/nvidia/generated
ccflags-y += -I$(NVROOT)/nvidia/inc
ccflags-y += -I$(NVROOT)/nvidia/inc/os
ccflags-y += -I$(NVROOT)/nvidia/inc/kernel
ccflags-y += -I$(NVROOT)/nvidia/inc/libraries
ccflags-y += -I$(NVROOT)/nvidia/src/libraries
ccflags-y += -I$(NVROOT)/nvidia/src/mm/uvm/interface
ccflags-y += -I$(NVROOT)/nvidia/arch/nvalloc/common/inc
ccflags-y += -I$(NVROOT)/nvidia/arch/nvalloc/common/inc/gsp
ccflags-y += -I$(NVROOT)/nvidia/arch/nvalloc/unix/include
ccflags-y += -I$(NVROOT)/nvidia-drm
ccflags-y += -I$(NVROOT)/nvidia-modeset
ccflags-y += -I$(NVROOT)/common/sdk/nvidia/inc
ccflags-y += -I$(NVROOT)/common/shared/inc
ccflags-y += -I$(NVROOT)/common/inc
ccflags-y += -I$(NVROOT)/common/inc/swref
ccflags-y += -I$(NVROOT)/common/inc/swref/published
ccflags-y += -I$(NVROOT)/common/inc/displayport
ccflags-y += -I$(NVROOT)/common/softfloat/nvidia
ccflags-y += -I$(NVROOT)/common/softfloat/source/include
ccflags-y += -I$(NVROOT)/common/softfloat/source/8086-SSE
ccflags-y += -I$(NVROOT)/common/unix/common/utils/interface
ccflags-y += -I$(NVROOT)/common/unix/common/inc
ccflags-y += -I$(NVROOT)/common/modeset
ccflags-y += -I$(NVROOT)/nvidia-modeset/os-interface/include
ccflags-y += -I$(NVROOT)/nvidia-modeset/kapi/interface
ccflags-y += -I$(NVROOT)/nvidia-modeset/interface
ccflags-y += -I$(NVROOT)/nvidia-modeset/include
ccflags-y += -I$(NVROOT)/nvidia-modeset/kapi/include
ccflags-y += -I$(NVROOT)/common/displayport/inc
ccflags-y += -I$(NVROOT)/common/displayport/inc/dptestutil
ccflags-y += -I$(NVROOT)/common/unix/nvidia-3d/interface
ccflags-y += -I$(NVROOT)/common/unix/nvidia-push/interface
ccflags-y += -I$(NVROOT)/common/unix/nvidia-3d/include
ccflags-y += -I$(NVROOT)/common/unix/nvidia-push/include
ccflags-y += -I$(NVROOT)/common/unix/xzminidec/interface
ccflags-y += -I$(NVROOT)/common/unix/nvidia-headsurface
ccflags-y += -I$(NVROOT)/nvidia-modeset/src/shaders
ccflags-y += -DNVRM -DNV_LINUX -DNV_KERNEL_INTERFACE_LAYER -DNV_VERSION_STRING=\"595.78\" -DNVIDIA_UNDEF_LEGACY_BIT_MACROS
ccflags-y += -DLIBSPDM_CONFIG=\"nvspdm_rmconfig.h\"
# feature switches from the original kernel-open Kbuild / nvidia-modeset.Kbuild
# (non-Android Linux; SPECTRE_V2_RETPOLINE defaults to 0)
ccflags-y += -DNV_SPECTRE_V2=0 -DNV_FILESYSTEM_ACCESS_AVAILABLE=1 -DNVKMS_CONFIG_FILE_SUPPORTED=1
# modeset stage-1 switches (nvdisplay/src/nvidia-modeset/Makefile): hdmipkt RM
# calls go through NVKMS callbacks, nv_smg.c uses nvkms-utils instead of pthread
ccflags-y += -DNVHDMIPKT_RM_CALLS_INTERNAL=0 -DNVHDMIPKT_NVKMS -DNV_SMG_IN_NVKMS
# remaining stage-1 defines
ccflags-y += -DNDEBUG -DNV_CPU_INTRINSICS_KERNEL -DNVT_USE_NVKMS
ccflags-y += -DNV_PUSH_IN_KERNEL -DNV_XZ_CUSTOM_MEM_HOOKS -DNV_XZ_USE_NVTYPES -DXZ_DEC_SINGLE
# xz.h auto-disables the built-in CRC32 under __KERNEL__, but nvidia-3d calls
# xz_crc32_init() unconditionally and xz_crc32.c is part of the module
ccflags-y += -DXZ_INTERNAL_CRC32=1
# softfloat fast-path switches (nvdisplay/src/nvidia-modeset/Makefile)
ccflags-y += -DSOFTFLOAT_ROUND_ODD -DSOFTFLOAT_FAST_DIV32TO16 -DSOFTFLOAT_FAST_DIV64TO32
# per-module defines from the original Kbuild fragments
ccflags-y += -UDEBUG -U_DEBUG -DNV_BUILD_MODULE_INSTANCES=0
# The original build targets plain armv8-a (no LSE); GCC/Clang then emit
# __aarch64_*_acq_rel helper calls for acq_rel atomics that the kernel does
# not export. T234/T264 cores (A78AE/Grace) are all LSE-capable (ARMv8.1+),
# so enable LSE atomics via the march feature (works for both GCC and Clang;
# -mlse-atomics is Clang-only). ARCH is exported to sub-makes.
ifeq ($(ARCH),arm64)
ccflags-y += -march=armv8-a+lse
endif
# Core RM (nvport/resserv) build configuration — required by nvport.h/nvassert.h
ccflags-y += -DPORT_IS_KERNEL_BUILD=1 -DPORT_IS_CHECKED_BUILD=0
ccflags-y += -DPORT_ASSERT_FAILED_USES_STRINGS=1 -DPORT_ATOMIC_64_BIT_SUPPORTED=1
ccflags-y += -DPORT_MODULE_atomic=1 -DPORT_MODULE_core=1 -DPORT_MODULE_cpu=1
ccflags-y += -DPORT_MODULE_crypto=1 -DPORT_MODULE_debug=1 -DPORT_MODULE_example=0
ccflags-y += -DPORT_MODULE_memory=1 -DPORT_MODULE_mmio=0 -DPORT_MODULE_safe=1
ccflags-y += -DPORT_MODULE_string=1 -DPORT_MODULE_sync=1 -DPORT_MODULE_thread=1
ccflags-y += -DPORT_MODULE_time=0 -DPORT_MODULE_util=1
ccflags-y += -DNV_ASSERT_FAILED_USES_STRINGS=1 -DNV_PRINTF_STRINGS_ALLOWED=1
ccflags-y += -DNV_CONTAINERS_NO_TEMPLATES -D__NO_CTYPE -D_LANGUAGE_C
ccflags-y += -DLOCK_VAL_ENABLED=0
ccflags-y += -DRS_COMPATABILITY_MODE=1 -DRS_PROVIDES_API_STATE=0 -DRS_STANDALONE=0 -DRS_STANDALONE_TEST=0
ccflags-y += -Wno-cast-qual -Wno-format-extra-args -Wno-strict-prototypes -Wno-unused-function

# The core RM sources (generated/, src/, hal/) are written against the
# standard C headers (stdarg.h, stdint.h, stdatomic.h, ...) which the kernel
# build excludes via -nostdinc. Expose the compiler's own internal standard
# headers (same as the original out-of-tree stage-1 build).
NV_CC_STDINC := $(shell $(CC) -print-file-name=include 2>/dev/null)
# compat-stdinc first: its self-contained stdint.h must shadow the compiler's
# (the compiler's stdint.h does #include_next into the C library, unavailable
# under -nostdinc). Other standard headers (stdarg.h, stddef.h, stdatomic.h)
# fall through to the compiler's internal dir.
ccflags-y += -I$(NVROOT)/compat-stdinc
ifneq ($(wildcard $(NV_CC_STDINC)/stdarg.h),)
ccflags-y += -I$(NV_CC_STDINC)
endif

# nv-procfs.c requires the build-generated nv_compiler.h
NV_COMPILER_VERSION_HEADER := $(obj)/nv_compiler.h
$(NV_COMPILER_VERSION_HEADER):
	@echo \#define NV_COMPILER \"`$(CC) -v 2>&1 | tail -n 1`\" > $@
$(obj)/nvidia/nv-procfs.o: $(NV_COMPILER_VERSION_HEADER)
targets += $(NV_COMPILER_VERSION_HEADER)
clean-files += $(NV_COMPILER_VERSION_HEADER)

$(info DBGVARS ARCH=[$(ARCH)] X86_64=[$(CONFIG_X86_64)])
