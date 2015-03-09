/*
 * PGD allocation/freeing
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Catalin Marinas <catalin.marinas@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/slab.h>

#include <asm/pgalloc.h>
#include <asm/page.h>
#include <asm/tlbflush.h>

#include "mm.h"

#define PGD_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))

pgd_t *pgd_alloc(struct mm_struct *mm)
{
	if (PGD_SIZE == PAGE_SIZE) {
		unsigned long page = get_zeroed_page(GFP_KERNEL);
		if (likely(kdp_enabled))
			kdp_protect_one_page((void *)page);
		return (pgd_t *)page;
	} else {
		return kzalloc(PGD_SIZE, GFP_KERNEL);
	}
}

void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	if (PGD_SIZE == PAGE_SIZE) {
		if (likely(kdp_enabled))
			kdp_unprotect_one_page(pgd);
		free_page((unsigned long)pgd);
	} else {
		kfree(pgd);
	}
}
