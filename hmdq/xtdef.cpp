/******************************************************************************
 * HMDQ - Query tool for an OpenVR HMD and some other hardware                *
 * https://github.com/risa2000/hmdq                                           *
 *                                                                            *
 * Copyright (c) 2019, Richard Musil. All rights reserved.                    *
 *                                                                            *
 * This source code is licensed under the BSD 3-Clause "New" or "Revised"     *
 * License found in the LICENSE file in the root directory of this project.   *
 * SPDX-License-Identifier: BSD-3-Clause                                      *
 ******************************************************************************/

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#include <xtensor/xview.hpp>

#include "except.h"
#include "xtdef.h"

//  functions
//------------------------------------------------------------------------------
//  Build the 2D-array from std::vector of 1D-arrays.
harray2d_t build_array(const hveclist_t& vecs)
{
    // at least one vector should always be present
    HMDQ_ASSERT(vecs.size() > 0);
    harray2d_t res({vecs.size(), vecs[0].shape(0)});
    for (size_t i = 0, e = vecs.size(); i < e; ++i) {
        xt::view(res, i) = vecs[i];
    }
    return res;
}
