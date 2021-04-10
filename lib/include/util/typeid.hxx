/* File author is Ítalo Lima Marconato Matias
 *
 * Created on March 04 of 2021, at 11:25 BRT
 * Last edited on March 15 of 2021, at 18:05 BRT */

#pragma once

#include <base/string.hxx>
#include <util/algo.hxx>

namespace CHicago {

class Type {
public:
    constexpr Type(const StringView &Name) : Name(Name), ID(ConstHash(Name.GetValue() + Name.GetViewStart(),
                                                                      Name.GetViewEnd() - Name.GetViewStart())) { }

    inline constexpr Boolean operator ==(const Type &Value) const { return ID == Value.ID; }
    inline constexpr Boolean operator !=(const Type &Value) const { return ID == Value.ID; }

    inline constexpr const StringView &GetName() const { return Name; }
    inline constexpr UInt64 GetID() const { return ID; }
private:
    StringView Name;
    UInt64 ID;
};

template<class> inline constexpr StringView NameOf() {
    return { __PRETTY_FUNCTION__ + 81, 0, sizeof(__PRETTY_FUNCTION__) - 83 };
}

template<class T> inline constexpr StringView NameOf(T) { return NameOf<T>(); }
template<class T> inline constexpr Type TypeOf(T) { return NameOf<T>(); }
template<class T> inline constexpr Type TypeOf() { return NameOf<T>(); }

}
