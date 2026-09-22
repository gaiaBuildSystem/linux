# SPDX-License-Identifier: GPL-2.0
NV_ROOT := /home/microhobby/projects/G/gaia/linux/drivers/gpu/nvidia

NVIDIA_SOURCES := $(shell grep -h 'NVIDIA_SOURCES *+=' $(NV_ROOT)/nvidia/nvidia-sources.Kbuild | sed 's/.*+= //')
NVIDIA_OBJECTS := $(patsubst %.c,%.o,$(NVIDIA_SOURCES))

obj-m += nvidia.o
nvidia-y := $(NVIDIA_OBJECTS)
ccflags-$(addprefix $(NV_ROOT)/nvidia/,$(NVIDIA_OBJECTS)) += \
	-I. -Iinclude -D__linux__ -DNV_LINUX
