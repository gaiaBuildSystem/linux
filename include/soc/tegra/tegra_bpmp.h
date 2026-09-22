/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Shim: nvidia's kernel-open display driver expects <soc/tegra/tegra_bpmp.h>
 * (the name used by the JetPack OOT build). The real Bpmp API lives in
 * <soc/tegra/bpmp.h> in this tree.
 */

#ifndef _TEGRA_BPMP_H_SHIM
#define _TEGRA_BPMP_H_SHIM

#include <soc/tegra/bpmp.h>

#endif /* _TEGRA_BPMP_H_SHIM */
