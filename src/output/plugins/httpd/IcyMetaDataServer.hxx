// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#ifndef MPD_ICY_META_DATA_SERVER_HXX
#define MPD_ICY_META_DATA_SERVER_HXX

#include "Page.hxx"
#include "tag/Type.h"

struct Tag;
class AllocatedString;

AllocatedString
icy_server_metadata_header(const char *name,
			   const char *genre, const char *url,
			   const char *content_type, int metaint) noexcept;

PagePtr
icy_server_metadata_page(const Tag &tag, const TagType *types) noexcept;

#endif
