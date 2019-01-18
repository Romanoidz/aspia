//
// Aspia Project
// Copyright (C) 2018 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include <gtest/gtest.h>

#include <QString>

#include "crypto/scoped_crypto_initializer.h"
#include "crypto/srp_constants.h"
#include "crypto/srp_math.h"

namespace crypto {

TEST(srp_math_test, test_vector)
{
    ScopedCryptoInitializer crypto_initializer;
    ASSERT_TRUE(crypto_initializer.isSucceeded());

    QString I = "alice";
    QString p = "password123";

    const uint8_t a_buf[] = { 0x60, 0x97, 0x55, 0x27, 0x03, 0x5C, 0xF2, 0xAD, 0x19, 0x89, 0x80,
                              0x6F, 0x04, 0x07, 0x21, 0x0B, 0xC8, 0x1E, 0xDC, 0x04, 0xE2, 0x76,
                              0x2A, 0x56, 0xAF, 0xD5, 0x29, 0xDD, 0xDA, 0x2D, 0x43, 0x93 };

    const uint8_t b_buf[] = { 0xE4, 0x87, 0xCB, 0x59, 0xD3, 0x1A, 0xC5, 0x50, 0x47, 0x1E, 0x81,
                              0xF0, 0x0F, 0x69, 0x28, 0xE0, 0x1D, 0xDA, 0x08, 0xE9, 0x74, 0xA0,
                              0x04, 0xF4, 0x9E, 0x61, 0xF5, 0xD1, 0x05, 0x28, 0x4D, 0x20 };

    const uint8_t s_buf[] = { 0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E, 0xB5, 0xA7, 0x27,
                              0x67, 0x3A, 0x24, 0x41, 0xEE };

    const uint8_t x_ref_buf[] =
    {
        0xA9, 0xB3, 0x2D, 0x47, 0x18, 0xF0, 0x0A, 0x4A, 0xA6, 0x1F, 0x7E, 0x24, 0xDE, 0xD3, 0xFF,
        0x16, 0xC3, 0xA5, 0x51, 0xED, 0xEF, 0x44, 0x12, 0xDE, 0xC0, 0x37, 0x83, 0xBE, 0x0B, 0xD3,
        0x46, 0x10, 0x79, 0xF8, 0x81, 0x66, 0xA4, 0x90, 0x2D, 0x13, 0xD3, 0x1C, 0xAB, 0x0C, 0x86,
        0xBC, 0x93, 0x6D, 0x93, 0xD2, 0x63, 0xBB, 0xC9, 0x97, 0x6F, 0x6E, 0x2C, 0x35, 0xD1, 0xE1,
        0x97, 0x72, 0xCA, 0x7D
    };

    const uint8_t u_ref_buf[] =
    {
        0x48, 0xA2, 0x66, 0x30, 0x1B, 0x9B, 0x4A, 0x2E, 0x9B, 0x13, 0xE0, 0x1B, 0xDF, 0xD0, 0xE4,
        0x8A, 0x20, 0x68, 0x14, 0xC3, 0xA5, 0xCE, 0x3C, 0x4E, 0xB5, 0xD6, 0xD4, 0x50, 0x3D, 0xC1,
        0x95, 0x58, 0x95, 0x0D, 0x06, 0xC7, 0x20, 0xDA, 0xC3, 0x99, 0x6B, 0x7A, 0xD7, 0x05, 0x0D,
        0x84, 0x39, 0xA8, 0xCB, 0x46, 0xFA, 0x07, 0x96, 0xF9, 0x85, 0x21, 0xC5, 0x8B, 0xFC, 0xC9,
        0x14, 0xEC, 0x11, 0x03
    };

    const uint8_t v_ref_buf[] =
    {
        0x8C, 0xB3, 0xF7, 0x72, 0xB6, 0x7D, 0x3D, 0x7B, 0x44, 0xD0, 0x78, 0x40, 0xB9, 0xBF, 0x0C,
        0x5F, 0x49, 0x2F, 0x7E, 0x85, 0xAA, 0x33, 0x7F, 0x96, 0xAF, 0xAC, 0xE5, 0xB1, 0x2F, 0x80,
        0x36, 0xAD, 0xA1, 0xBB, 0x76, 0x58, 0x37, 0x53, 0xFC, 0xA8, 0x98, 0x58, 0xDE, 0xA5, 0xF3,
        0x3C, 0x64, 0x62, 0x08, 0xE3, 0x1B, 0x72, 0xBD, 0x6F, 0xE9, 0xC2, 0x7E, 0x4F, 0xF5, 0xE6,
        0x8F, 0x21, 0x1B, 0x52, 0xA1, 0x72, 0x6C, 0xAC, 0xD9, 0xBB, 0xB8, 0x85, 0xB0, 0xF0, 0xC3,
        0x27, 0xB3, 0x87, 0xA6, 0x58, 0xA8, 0x4C, 0x45, 0xB2, 0x7A, 0x10, 0x84, 0x88, 0xB1, 0xBA,
        0xC6, 0x61, 0x87, 0x44, 0xAF, 0x56, 0x75, 0x26, 0x67, 0x95, 0xF6, 0x29, 0x41, 0xDF, 0xEC,
        0xC7, 0xA6, 0x1F, 0x4C, 0xE5, 0x9B, 0x4F, 0x9C, 0x1F, 0x92, 0xDC, 0xA5, 0xE4, 0xD4, 0xAF,
        0xFE, 0x6C, 0x2D, 0x4C, 0xBC, 0xB7, 0x66, 0x27
    };

    const uint8_t A_ref_buf[] =
    {
        0x61, 0xD5, 0xE4, 0x90, 0xF6, 0xF1, 0xB7, 0x95, 0x47, 0xB0, 0x70, 0x4C, 0x43, 0x6F, 0x52,
        0x3D, 0xD0, 0xE5, 0x60, 0xF0, 0xC6, 0x41, 0x15, 0xBB, 0x72, 0x55, 0x7E, 0xC4, 0x43, 0x52,
        0xE8, 0x90, 0x32, 0x11, 0xC0, 0x46, 0x92, 0x27, 0x2D, 0x8B, 0x2D, 0x1A, 0x53, 0x58, 0xA2,
        0xCF, 0x1B, 0x6E, 0x0B, 0xFC, 0xF9, 0x9F, 0x92, 0x15, 0x30, 0xEC, 0x8E, 0x39, 0x35, 0x61,
        0x79, 0xEA, 0xE4, 0x5E, 0x42, 0xBA, 0x92, 0xAE, 0xAC, 0xED, 0x82, 0x51, 0x71, 0xE1, 0xE8,
        0xB9, 0xAF, 0x6D, 0x9C, 0x03, 0xE1, 0x32, 0x7F, 0x44, 0xBE, 0x08, 0x7E, 0xF0, 0x65, 0x30,
        0xE6, 0x9F, 0x66, 0x61, 0x52, 0x61, 0xEE, 0xF5, 0x40, 0x73, 0xCA, 0x11, 0xCF, 0x58, 0x58,
        0xF0, 0xED, 0xFD, 0xFE, 0x15, 0xEF, 0xEA, 0xB3, 0x49, 0xEF, 0x5D, 0x76, 0x98, 0x8A, 0x36,
        0x72, 0xFA, 0xC4, 0x7B, 0x07, 0x69, 0x44, 0x7B
    };

    const uint8_t B_ref_buf[] =
    {
        0x4F, 0x05, 0x71, 0xED, 0x14, 0xE6, 0x8B, 0x76, 0xB4, 0x99, 0x9C, 0x23, 0x80, 0x93, 0x04,
        0x81, 0x87, 0x50, 0x1C, 0x58, 0xB9, 0xFD, 0xF6, 0x53, 0xC0, 0x68, 0x37, 0x92, 0x4A, 0x8F,
        0x99, 0x6A, 0x54, 0x3D, 0x72, 0xC7, 0x34, 0xEC, 0x67, 0x93, 0xF9, 0xFF, 0xFB, 0xA7, 0xFB,
        0x65, 0xC1, 0xEB, 0xC5, 0xD8, 0xE2, 0x8A, 0xD0, 0xAE, 0xB2, 0x1C, 0xDB, 0x86, 0xD7, 0x1E,
        0xD3, 0x66, 0x73, 0xD7, 0x18, 0x78, 0xF0, 0xD3, 0xD8, 0x51, 0x77, 0x81, 0xAE, 0x67, 0x6F,
        0x16, 0x16, 0x4E, 0x8E, 0x71, 0x11, 0xC1, 0x62, 0xFB, 0xED, 0x32, 0xFA, 0x6B, 0xCC, 0x3E,
        0x88, 0x28, 0x72, 0x22, 0xA4, 0x34, 0xDE, 0xD9, 0xF5, 0xE5, 0x35, 0x20, 0x59, 0xCD, 0x91,
        0xFF, 0xC8, 0x39, 0xD6, 0xD3, 0xC6, 0xBC, 0xE0, 0xEB, 0xA4, 0x02, 0x7C, 0xC0, 0x4B, 0x42,
        0x44, 0xE2, 0x7D, 0x72, 0x3B, 0xE2, 0xE1, 0x97
    };

    const uint8_t key_ref_buf[] =
    {
        0xD9, 0xD6, 0xC9, 0xAB, 0xF2, 0x77, 0xB0, 0x05, 0x2E, 0x40, 0xDF, 0x3A, 0x03, 0x13, 0x58,
        0xC0, 0x31, 0x66, 0xE7, 0xF8, 0x15, 0x42, 0x5C, 0xCA, 0x1A, 0xBA, 0xAA, 0x63, 0x73, 0x03,
        0x18, 0x47, 0x31, 0xDC, 0xDF, 0xDC, 0xC8, 0xC7, 0x86, 0xEA, 0x16, 0x47, 0x66, 0xEE, 0xAB,
        0x9F, 0xCD, 0x13, 0x5C, 0x9C, 0xF8, 0xF6, 0xF7, 0xEB, 0x0D, 0xA8, 0xF3, 0x06, 0x54, 0x38,
        0x81, 0xA2, 0x2F, 0x06, 0xA0, 0xB6, 0xEB, 0xD7, 0x49, 0x8C, 0x72, 0xBC, 0xC5, 0x20, 0x61,
        0x4E, 0x5A, 0xF2, 0x21, 0xDA, 0xFB, 0x2F, 0xBA, 0x19, 0xF4, 0x5D, 0xA6, 0x94, 0x3F, 0x95,
        0x52, 0xBB, 0x65, 0xFC, 0xB3, 0x81, 0x9C, 0x66, 0x77, 0x1D, 0xC1, 0x80, 0xDD, 0x08, 0x3D,
        0xA8, 0x22, 0x86, 0x2C, 0x4C, 0xD7, 0xD0, 0x9D, 0x9E, 0x26, 0x0C, 0x5F, 0x76, 0x57, 0xD0,
        0x6D, 0x40, 0xFC, 0xBD, 0x14, 0xBC, 0xED, 0x40
    };

    static_assert(sizeof(a_buf) == 32);
    static_assert(sizeof(b_buf) == 32);
    static_assert(sizeof(s_buf) == 16);
    static_assert(sizeof(x_ref_buf) == 64);
    static_assert(sizeof(u_ref_buf) == 64);
    static_assert(sizeof(v_ref_buf) == 128);
    static_assert(sizeof(A_ref_buf) == 128);
    static_assert(sizeof(B_ref_buf) == 128);
    static_assert(sizeof(key_ref_buf) == 128);

    BigNum N = BigNum::fromBuffer(kSrpNg_1024.N);
    BigNum g = BigNum::fromBuffer(kSrpNg_1024.g);
    BigNum s = BigNum::fromBuffer(base::ConstBuffer(s_buf, sizeof(s_buf)));
    ASSERT_TRUE(N.isValid());
    ASSERT_TRUE(g.isValid());
    ASSERT_TRUE(s.isValid());

    BigNum v_ref = BigNum::fromBuffer(
        base::ConstBuffer(reinterpret_cast<const uint8_t*>(v_ref_buf), sizeof(v_ref_buf)));
    ASSERT_TRUE(v_ref.isValid());

    BigNum a = BigNum::fromBuffer(base::ConstBuffer(a_buf, sizeof(a_buf)));
    BigNum b = BigNum::fromBuffer(base::ConstBuffer(b_buf, sizeof(b_buf)));

    ASSERT_TRUE(a.isValid());
    ASSERT_TRUE(b.isValid());

    BigNum A = SrpMath::calc_A(a, N, g);
    ASSERT_TRUE(A.isValid());

    std::string A_string = A.toStdString();
    ASSERT_EQ(A_string.size(), sizeof(A_ref_buf));
    ASSERT_EQ(memcmp(A_string.c_str(), A_ref_buf, sizeof(A_ref_buf)), 0);

    ASSERT_TRUE(SrpMath::verify_A_mod_N(A, N));

    BigNum B = SrpMath::calc_B(b, N, g, v_ref);
    ASSERT_TRUE(B.isValid());

    std::string B_string = B.toStdString();
    ASSERT_EQ(B_string.size(), sizeof(B_ref_buf));
    ASSERT_EQ(memcmp(B_string.c_str(), B_ref_buf, sizeof(B_ref_buf)), 0);

    ASSERT_TRUE(SrpMath::verify_B_mod_N(B, N));

    BigNum u = SrpMath::calc_u(A, B, N);
    ASSERT_TRUE(u.isValid());

    std::string u_string = u.toStdString();
    ASSERT_EQ(u_string.size(), sizeof(u_ref_buf));
    ASSERT_EQ(memcmp(u_string.c_str(), u_ref_buf, sizeof(u_ref_buf)), 0);

    BigNum x = SrpMath::calc_x(s, I.toUtf8(), p.toUtf8());
    ASSERT_TRUE(x.isValid());

    std::string x_string = x.toStdString();
    ASSERT_EQ(x_string.size(), sizeof(x_ref_buf));
    ASSERT_EQ(memcmp(x_string.c_str(), x_ref_buf, sizeof(x_ref_buf)), 0);

    BigNum v = SrpMath::calc_v(I.toUtf8(), p.toUtf8(), s, N, g);
    ASSERT_TRUE(v.isValid());

    std::string v_string = v.toStdString();
    ASSERT_EQ(v_string.size(), sizeof(v_ref_buf));
    ASSERT_EQ(memcmp(v_string.c_str(), v_ref_buf, sizeof(v_ref_buf)), 0);

    BigNum server_key = SrpMath::calcServerKey(A, v, u, b, N);
    ASSERT_TRUE(server_key.isValid());

    std::string server_key_string = server_key.toStdString();
    ASSERT_EQ(server_key_string.size(), sizeof(key_ref_buf));
    ASSERT_EQ(memcmp(server_key_string.c_str(), key_ref_buf, sizeof(key_ref_buf)), 0);

    BigNum client_key = SrpMath::calcClientKey(N, B, g, x, a, u);
    ASSERT_TRUE(client_key.isValid());

    std::string client_key_string = client_key.toStdString();
    ASSERT_EQ(client_key_string.size(), sizeof(key_ref_buf));
    ASSERT_EQ(memcmp(client_key_string.c_str(), key_ref_buf, sizeof(key_ref_buf)), 0);
}

} // namespace crypto
