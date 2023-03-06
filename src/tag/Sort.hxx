// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#pragma once

#include "Type.h"

struct Tag;

[[gnu::pure]]
bool
CompareTags(TagType type, bool descending,
	    const Tag &a, const Tag &b) noexcept;
