// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "IcyInputStream.hxx"
#include "tag/IcyMetaDataParser.hxx"
#include "tag/Tag.hxx"
#include "util/UriExtract.hxx"
#include "util/UriQueryParser.hxx"

#include <string>

IcyInputStream::IcyInputStream(InputStreamPtr _input,
			       std::shared_ptr<IcyMetaDataParser> _parser)
	:ProxyInputStream(std::move(_input)), parser(std::move(_parser))
{
#ifdef HAVE_ICU_CONVERTER
	const char *fragment = uri_get_fragment(GetURI());
	if (fragment != nullptr) {
		const auto charset = UriFindRawQueryParameter(fragment,
							      "charset");
		if (charset.data() != nullptr)
			parser->SetCharset(std::string{charset}.c_str());
	}
#endif
}

IcyInputStream::~IcyInputStream() noexcept = default;

inline bool
IcyInputStream::IsEnabled() const noexcept
{
	assert(parser);

	return parser->IsDefined();
}

void
IcyInputStream::Update() noexcept
{
	ProxyInputStream::Update();

	if (IsEnabled())
		offset = override_offset;
}

std::unique_ptr<Tag>
IcyInputStream::ReadTag() noexcept
{
	auto new_input_tag = ProxyInputStream::ReadTag();
	if (!IsEnabled())
		return new_input_tag;

	const bool had_new_input_tag = !!new_input_tag;
	if (new_input_tag != nullptr)
		input_tag = std::move(new_input_tag);

	auto new_icy_tag = parser->ReadTag();
	const bool had_new_icy_tag = !!new_icy_tag;
	if (new_icy_tag != nullptr)
		icy_tag = std::move(new_icy_tag);

	if (!had_new_input_tag && !had_new_icy_tag)
		/* no change */
		return nullptr;

	return Tag::Merge(input_tag.get(), icy_tag.get());
}

size_t
IcyInputStream::Read(std::unique_lock<Mutex> &lock,
		     void *ptr, size_t read_size)
{
	if (!IsEnabled())
		return ProxyInputStream::Read(lock, ptr, read_size);

	while (true) {
		size_t nbytes = ProxyInputStream::Read(lock, ptr, read_size);
		if (nbytes == 0) {
			assert(IsEOF());
			offset = override_offset;
			return 0;
		}

		size_t result = parser->ParseInPlace(ptr, nbytes);
		if (result > 0) {
			override_offset += result;
			offset = override_offset;
			return result;
		}
	}
}
