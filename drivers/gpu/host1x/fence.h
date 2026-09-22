/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2020, NVIDIA Corporation.
 */

#ifndef HOST1X_FENCE_H
#define HOST1X_FENCE_H

struct host1x_syncpt_fence {
	struct dma_fence base;

	atomic_t signaling;

	struct host1x_syncpt *sp;
	u32 threshold;
	bool timeout;

	struct delayed_work timeout_work;

	struct list_head list;
};

struct host1x_fence_list {
	spinlock_t lock;
	struct list_head list;
};

void host1x_fence_signal(struct host1x_syncpt_fence *fence);

struct dma_fence *host1x_fence_create(struct host1x_syncpt *sp, u32 threshold,
				      bool timeout);
int host1x_fence_extract(struct dma_fence *fence, u32 *id, u32 *threshold);
int host1x_fence_get_node(struct dma_fence *fence);
void host1x_fence_cancel(struct dma_fence *fence);

#endif
