// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#ifndef MPD_PLAYLIST_REGISTRY_HXX
#define MPD_PLAYLIST_REGISTRY_HXX

#include "input/Ptr.hxx"
#include "thread/Mutex.hxx"

#include <string_view>

struct ConfigData;
struct PlaylistPlugin;
class SongEnumerator;

extern const PlaylistPlugin *const playlist_plugins[];

#define playlist_plugins_for_each(plugin) \
	for (const PlaylistPlugin *plugin, \
		*const*playlist_plugin_iterator = &playlist_plugins[0]; \
		(plugin = *playlist_plugin_iterator) != nullptr; \
		++playlist_plugin_iterator)

/**
 * Initializes all playlist plugins.
 */
void
playlist_list_global_init(const ConfigData &config);

/**
 * Deinitializes all playlist plugins.
 */
void
playlist_list_global_finish() noexcept;

class ScopePlaylistPluginsInit {
public:
	explicit ScopePlaylistPluginsInit(const ConfigData &config) {
		playlist_list_global_init(config);
	}

	~ScopePlaylistPluginsInit() noexcept {
		playlist_list_global_finish();
	}
};

/**
 * Shall this playlists supported by this plugin be represented as
 * directories in the database?
 */
[[gnu::const]]
bool
GetPlaylistPluginAsFolder(const PlaylistPlugin &plugin) noexcept;

/**
 * Opens a playlist by its URI.
 */
std::unique_ptr<SongEnumerator>
playlist_list_open_uri(const char *uri, Mutex &mutex);

std::unique_ptr<SongEnumerator>
playlist_list_open_stream_suffix(InputStreamPtr &&is, std::string_view suffix);

/**
 * Opens a playlist from an input stream.
 *
 * @param is an #InputStream object which is open and ready
 * @param uri optional URI which was used to open the stream; may be
 * used to select the appropriate playlist plugin
 */
std::unique_ptr<SongEnumerator>
playlist_list_open_stream(InputStreamPtr &&is, const char *uri);

[[gnu::pure]]
const PlaylistPlugin *
FindPlaylistPluginBySuffix(std::string_view suffix) noexcept;

/**
 * Determines if there is a playlist plugin which can handle the
 * specified file name suffix.
 */
[[gnu::pure]]
inline bool
playlist_suffix_supported(std::string_view suffix) noexcept
{
	return FindPlaylistPluginBySuffix(suffix) != nullptr;
}

#endif
