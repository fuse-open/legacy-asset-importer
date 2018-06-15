#include "../catch.hpp"
#include <Xli/Unicode.h>

using namespace Xli;

TEST_CASE("ToUpper")
{
    REQUIRE(0 == Unicode::ToUpper(0));
    REQUIRE('.' == Unicode::ToUpper('.'));
    REQUIRE('A' == Unicode::ToUpper('a'));
    REQUIRE('A' == Unicode::ToUpper('A'));
    REQUIRE(0xc5 == Unicode::ToUpper(0xe5)); //æ
}

TEST_CASE("ToLower")
{
    REQUIRE(0 == Unicode::ToLower(0));
    REQUIRE('.' == Unicode::ToLower('.'));
    REQUIRE('a' == Unicode::ToLower('a'));
    REQUIRE('a' == Unicode::ToLower('A'));
    REQUIRE(0xe5 == Unicode::ToLower(0xc5)); //æ
}

TEST_CASE("ModifiedUtf8")
{
    Utf16String utf16 = Unicode::Utf8To16(String("\0\0foo\0\0bar\0\0", 12));
    String mutf8 = Unicode::Utf16To8(utf16, UnicodeFlagsModifiedUtf8);
    REQUIRE(mutf8.Length() == 18);
    REQUIRE(mutf8[0] == (char)(unsigned char)0xC0);
    REQUIRE(mutf8[1] == (char)(unsigned char)0x80);

    Utf16String utf16_3 = Unicode::Utf8To16(mutf8, UnicodeFlagsModifiedUtf8);
    REQUIRE(Unicode::Utf16To8(utf16_3, 0).Length() == 12);
    REQUIRE(Unicode::Utf16To8(utf16_3, UnicodeFlagsModifiedUtf8).Length() == 18);

    Utf16String utf16_2 = Unicode::Utf8To16(mutf8, UnicodeFlagsIgnoreError);
    REQUIRE(Unicode::Utf16To8(utf16_2).Length() == 0);
}
