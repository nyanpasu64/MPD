// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#ifndef MPD_DB_UNIQUE_TAGS_HXX
#define MPD_DB_UNIQUE_TAGS_HXX

#include "tag/Type.h"

#include <span>
#include <string>

class Database;
struct DatabaseSelection;
template<typename Key> class RecursiveMap;

/**
 * Walk the database and collect unique tag values.
 */
RecursiveMap<std::string>
CollectUniqueTags(const Database &db, const DatabaseSelection &selection,
		  std::span<const TagType> tag_types);

#endif
