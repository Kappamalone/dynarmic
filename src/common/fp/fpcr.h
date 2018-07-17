/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#pragma once

#include <boost/optional.hpp>

#include "common/assert.h"
#include "common/bit_util.h"
#include "common/common_types.h"
#include "common/fp/rounding_mode.h"

namespace Dynarmic::FP {

/**
 * Representation of the Floating-Point Control Register.
 */
class FPCR final {
public:
    FPCR() = default;
    FPCR(const FPCR&) = default;
    FPCR(FPCR&&) = default;
    explicit FPCR(u32 data) : value{data & mask} {}

    FPCR& operator=(const FPCR&) = default;
    FPCR& operator=(FPCR&&) = default;
    FPCR& operator=(u32 data) {
        value = data & mask;
        return *this;
    }

    /// Get alternate half-precision control flag.
    bool AHP() const {
        return Common::Bit<26>(value);
    }

    /// Set alternate half-precision control flag.
    void AHP(bool ahp) {
        value = Common::ModifyBit<26>(value, ahp);
    }

    /// Get default NaN mode control bit.
    bool DN() const {
        return Common::Bit<25>(value);
    }

    /// Set default NaN mode control bit.
    void DN(bool dn) {
        value = Common::ModifyBit<25>(value, dn);
    }

    /// Get flush-to-zero mode control bit.
    bool FZ() const {
        return Common::Bit<24>(value);
    }

    /// Set flush-to-zero mode control bit.
    void FZ(bool fz) {
        value = Common::ModifyBit<24>(value, fz);
    }

    /// Get rounding mode control field.
    FP::RoundingMode RMode() const {
        return static_cast<FP::RoundingMode>(Common::Bits<22, 23>(value));
    }

    /// Set rounding mode control field.
    void RMode(FP::RoundingMode rounding_mode) {
        ASSERT_MSG(static_cast<u32>(rounding_mode) <= 0b11, "FPCR: Invalid rounding mode");
        value = Common::ModifyBits<22, 23>(value, static_cast<u32>(rounding_mode));
    }

    /// Get the stride of a vector when executing AArch32 VFP instructions.
    /// This field has no function in AArch64 state.
    boost::optional<size_t> Stride() const {
        switch (Common::Bits<20, 21>(value)) {
        case 0b00:
            return 1;
        case 0b11:
            return 2;
        default:
            return boost::none;
        }
    }

    /// Set the stride of a vector when executing AArch32 VFP instructions.
    /// This field has no function in AArch64 state.
    void Stride(size_t stride) {
        ASSERT_MSG(stride >= 1 && stride <= 2, "FPCR: Invalid stride");
        value = Common::ModifyBits<20, 21>(value, stride == 1 ? 0b00u : 0b11u);
    }

    /// Get flush-to-zero (half-precision specific) mode control bit.
    bool FZ16() const {
        return Common::Bit<19>(value);
    }

    /// Set flush-to-zero (half-precision specific) mode control bit.
    void FZ16(bool fz16) {
        value = Common::ModifyBit<19>(value, fz16);
    }

    /// Gets the length of a vector when executing AArch32 VFP instructions.
    /// This field has no function in AArch64 state.
    size_t Len() const {
        return Common::Bits<16, 18>(value) + 1;
    }

    /// Sets the length of a vector when executing AArch32 VFP instructions.
    /// This field has no function in AArch64 state.
    void Len(size_t len) {
        ASSERT_MSG(len >= 1 && len <= 8, "FPCR: Invalid len");
        value = Common::ModifyBits<16, 18>(value, static_cast<u32>(len - 1));
    }

    /// Get input denormal exception trap enable flag.
    bool IDE() const {
        return Common::Bit<15>(value);
    }

    /// Set input denormal exception trap enable flag.
    void IDE(bool ide) {
        value = Common::ModifyBit<15>(value, ide);
    }

    /// Get inexact exception trap enable flag.
    bool IXE() const {
        return Common::Bit<12>(value);
    }

    /// Set inexact exception trap enable flag.
    void IXE(bool ixe) {
        value = Common::ModifyBit<12>(value, ixe);
    }

    /// Get underflow exception trap enable flag.
    bool UFE() const {
        return Common::Bit<11>(value);
    }

    /// Set underflow exception trap enable flag.
    void UFE(bool ufe) {
        value = Common::ModifyBit<11>(value, ufe);
    }

    /// Get overflow exception trap enable flag.
    bool OFE() const {
        return Common::Bit<10>(value);
    }

    /// Set overflow exception trap enable flag.
    void OFE(bool ofe) {
        value = Common::ModifyBit<10>(value, ofe);
    }

    /// Get division by zero exception trap enable flag.
    bool DZE() const {
        return Common::Bit<9>(value);
    }

    /// Set division by zero exception trap enable flag.
    void DZE(bool dze) {
        value = Common::ModifyBit<9>(value, dze);
    }

    /// Get invalid operation exception trap enable flag.
    bool IOE() const {
        return Common::Bit<8>(value);
    }

    /// Set invalid operation exception trap enable flag.
    void IOE(bool ioe) {
        value = Common::ModifyBit<8>(value, ioe);
    }

    /// Gets the underlying raw value within the FPCR.
    u32 Value() const {
        return value;
    }

private:
    // Bits 0-7, 13-14, and 27-31 are reserved.
    static constexpr u32 mask = 0x07FF9F00;
    u32 value = 0;
};

inline bool operator==(FPCR lhs, FPCR rhs) {
    return lhs.Value() == rhs.Value();
}

inline bool operator!=(FPCR lhs, FPCR rhs) {
    return !operator==(lhs, rhs);
}

} // namespace Dynarmic::FP
