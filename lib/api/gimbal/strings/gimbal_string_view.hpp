#ifndef GIMBAL_STRING_VIEW_HPP
#define GIMBAL_STRING_VIEW_HPP

#include "gimbal_string_view.h"
#include "gimbal_quark.hpp"
#include <string_view>
#include <functional>
#include <compare>
#include <type_traits>
#include <optional>

namespace gbl {

class StringView;

template<typename T>
std::optional<T> string_view_to_value(const StringView& view) {
    return std::nullopt;
}

struct StringView: public GblStringView {

    constexpr StringView() noexcept:
        GblStringView({0}) {}

    constexpr StringView(const char* pStr, size_t size=0) noexcept:
        GblStringView({ .pData = pStr })
    {
        if(!size) {
            static_cast<GblStringView*>(this)->length = std::char_traits<char>::length(pStr);
            this->nullTerminated = true;
        } else {
            static_cast<GblStringView*>(this)->length = size;
            this->nullTerminated = false;
        }
    }

    constexpr StringView(const StringView& rhs) noexcept:
        GblStringView(rhs) {}

    constexpr StringView(GblStringView gblStrv) noexcept:
        GblStringView(gblStrv) {}

    constexpr StringView(std::string_view view) noexcept:
        StringView(view.data(), view.length()) {}

    constexpr const char* data() const noexcept { return pData; }
    constexpr std::size_t length() const noexcept { return static_cast<const GblStringView*>(this)->length; }
    constexpr bool isNullTerminated() const noexcept { return nullTerminated; }

    int compareIgnoreCase(const char* pString, std::size_t len=0) const noexcept {
        return GblStringView_compareIgnoreCase(*this, pString, len);
    }

    bool equalsIgnoreCase(const char* pString, std::size_t len=0) const noexcept {
        return GblStringView_equalsIgnoreCase(*this, pString, len);
    }

    //rethrow
    void copy(void* pDst, std::size_t offset, std::size_t bytes) const noexcept {
        GblStringView_copy(*this, pDst, offset, bytes);
    }

    bool isEmpty() const noexcept {
        return GblStringView_empty(*this);
    }

    bool isBlank() const noexcept {
        return GblStringView_blank(*this);
    }

    constexpr char operator[](std::size_t index) const {
        if(index >= length())
            throw std::out_of_range("Invalid StringView index!");
        if (std::is_constant_evaluated()) {
            return data()[index];
        } else {
            return GblStringView_at(*this, index);
        }
    }

    char first() const {
        if(isEmpty())
            throw std::out_of_range("Cannot get first char of empty StringView!");

        return GblStringView_first(*this);
    }

    char last() const {
        if(isEmpty())
            throw std::out_of_range("Cannot get last char of empty StringView!");

        return GblStringView_last(*this);
    }

    StringView removePrefix(std::size_t len) const {
        if(len > length())
            throw std::out_of_range("Cannot remove prefix length > StringView.length!");
        return GblStringView_removePrefix(*this, len);

    }

    StringView removeSuffix(std::size_t len) const {
        if(len > length())
            throw std::out_of_range("Cannot remove suffix length > StringView.length!");
        return GblStringView_removeSuffix(*this, len);

    }

    StringView chomp() const noexcept {
        return GblStringView_chomp(*this);
    }

    StringView substr(std::size_t offset, std::size_t length) const noexcept {
        return GblStringView_substr(*this, offset, length);
    }

    bool contains(const char* pStr, std::size_t len=0) const noexcept {
        return GblStringView_contains(*this, pStr, len);
    }

    bool containsIgnoreCase(const char* pStr, std::size_t len=0) const noexcept {
        return GblStringView_containsIgnoreCase(*this, pStr, len);
    }

    std::size_t count(const char* pStr, std::size_t len=0) const noexcept {
        return GblStringView_count(*this, pStr, len);
    }

    std::size_t countIgnoreCase(const char* pStr, std::size_t len=0) const noexcept {
        return GblStringView_countIgnoreCase(*this, pStr, len);
    }

    Quark quark() const noexcept {
        return GblStringView_quark(*this);
    }

    Quark tryQuark() const noexcept {
        return GblStringView_tryQuark(*this);
    }

    const char* intern() const noexcept {
        return GblStringView_intern(*this);
    }

    template<typename T>
    auto toValue() const noexcept{ return string_view_to_value<T>(*this); }

    friend std::ostream& operator<<(std::ostream &os, StringView view) {
        os << std::string_view(view.data(), view.length());
        return os;
    }

    bool operator==(StringView rhs) const noexcept {
        return GblStringView_equals(*this, rhs.data(), rhs.length());
    }

    auto operator<=>(StringView rhs) const noexcept {
        const auto result = GblStringView_compare(*this, rhs.data(), rhs.length());

        if(result < 0)
            return std::strong_ordering::less;
        else if(result > 0)
            return std::strong_ordering::greater;
        else
            return std::strong_ordering::equal;
    }

};

#define GBL_STRING_VIEW_TO_VALUE_(type, postfix) \
    template<> \
    std::optional<type> string_view_to_value<type>(const StringView& view) { \
        GblBool ok; \
        type value = GblStringView_##postfix(view, &ok); \
        return ok? std::optional{value} : std::nullopt; \
    }

GBL_STRING_VIEW_TO_VALUE_(bool, toBool)
GBL_STRING_VIEW_TO_VALUE_(char, toChar)
GBL_STRING_VIEW_TO_VALUE_(uint8_t, toUint8)
GBL_STRING_VIEW_TO_VALUE_(uint16_t, toUint16)
GBL_STRING_VIEW_TO_VALUE_(int16_t, toInt16)
GBL_STRING_VIEW_TO_VALUE_(uint32_t, toUint32)
GBL_STRING_VIEW_TO_VALUE_(int32_t, toInt32)
GBL_STRING_VIEW_TO_VALUE_(uint64_t, toUint64)
GBL_STRING_VIEW_TO_VALUE_(int64_t, toInt64)
GBL_STRING_VIEW_TO_VALUE_(float, toFloat)
GBL_STRING_VIEW_TO_VALUE_(double, toDouble)
GBL_STRING_VIEW_TO_VALUE_(void*, toPointer)

#undef GBL_STRING_VIEW_TO_VALUE_

consteval inline StringView operator""_strv(const char* pString, std::size_t size) noexcept {
    return StringView(pString, size);
}

}

template<>
struct std::hash<gbl::StringView> {
    std::size_t operator()(const gbl::StringView& view) const noexcept {
        return GblStringView_hash(view);
    }
};

#endif // GIMBAL_STRING_VIEW_HPP
