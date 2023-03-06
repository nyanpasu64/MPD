// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "test_pcm_util.hxx"
#include "pcm/PcmChannels.hxx"
#include "pcm/Buffer.hxx"

#include <gtest/gtest.h>

TEST(PcmTest, Channels16)
{
	constexpr size_t N = 509;
	const auto src = TestDataBuffer<int16_t, N * 2>();

	PcmBuffer buffer;

	/* stereo to mono */

	auto dest = pcm_convert_channels_16(buffer, 1, 2, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N, dest.size());
	for (unsigned i = 0; i < N; ++i)
		EXPECT_EQ(int16_t((src[i * 2] + src[i * 2 + 1]) / 2),
			  dest[i]);

	/* mono to stereo */

	dest = pcm_convert_channels_16(buffer, 2, 1, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N * 4, dest.size());
	for (unsigned i = 0; i < N; ++i) {
		EXPECT_EQ(src[i], dest[i * 2]);
		EXPECT_EQ(src[i], dest[i * 2 + 1]);
	}

	/* stereo to 5.1 */

	dest = pcm_convert_channels_16(buffer, 6, 2, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N * 6, dest.size());
	constexpr int16_t silence = 0;
	for (unsigned i = 0; i < N; ++i) {
		EXPECT_EQ(src[i * 2], dest[i * 6]);
		EXPECT_EQ(src[i * 2 + 1], dest[i * 6+ 1]);
		EXPECT_EQ(silence, dest[i * 6 + 2]);
		EXPECT_EQ(silence, dest[i * 6 + 3]);
		EXPECT_EQ(silence, dest[i * 6 + 4]);
		EXPECT_EQ(silence, dest[i * 6 + 5]);
	}
}

TEST(PcmTest, Channels32)
{
	constexpr size_t N = 509;
	const auto src = TestDataBuffer<int32_t, N * 2>();

	PcmBuffer buffer;

	/* stereo to mono */

	auto dest = pcm_convert_channels_32(buffer, 1, 2, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N, dest.size());
	for (unsigned i = 0; i < N; ++i)
		EXPECT_EQ(int32_t(((int64_t)src[i * 2] + (int64_t)src[i * 2 + 1]) / 2),
			  dest[i]);

	/* mono to stereo */

	dest = pcm_convert_channels_32(buffer, 2, 1, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N * 4, dest.size());
	for (unsigned i = 0; i < N; ++i) {
		EXPECT_EQ(src[i], dest[i * 2]);
		EXPECT_EQ(src[i], dest[i * 2 + 1]);
	}

	/* stereo to 5.1 */

	dest = pcm_convert_channels_32(buffer, 6, 2, src);
	EXPECT_NE(dest.data(), nullptr);
	EXPECT_EQ(N * 6, dest.size());
	constexpr int32_t silence = 0;
	for (unsigned i = 0; i < N; ++i) {
		EXPECT_EQ(src[i * 2], dest[i * 6]);
		EXPECT_EQ(src[i * 2 + 1], dest[i * 6+ 1]);
		EXPECT_EQ(silence, dest[i * 6 + 2]);
		EXPECT_EQ(silence, dest[i * 6 + 3]);
		EXPECT_EQ(silence, dest[i * 6 + 4]);
		EXPECT_EQ(silence, dest[i * 6 + 5]);
	}
}
