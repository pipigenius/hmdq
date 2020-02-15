/******************************************************************************
 * HMDQ Tools - tools for an OpenVR HMD and other hardware introspection      *
 * https://github.com/risa2000/hmdq                                           *
 *                                                                            *
 * Copyright (c) 2019, Richard Musil. All rights reserved.                    *
 *                                                                            *
 * This source code is licensed under the BSD 3-Clause "New" or "Revised"     *
 * License found in the LICENSE file in the root directory of this project.   *
 * SPDX-License-Identifier: BSD-3-Clause                                      *
 ******************************************************************************/

#pragma once

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#include <string>
#include <vector>

#define OPENVR_BUILD_STATIC
#include <openvr/openvr.h>

//  typedefs
//------------------------------------------------------------------------------
typedef std::vector<std::pair<vr::EVREye, std::string>> heyes_t;
typedef std::vector<vr::ETrackedDeviceProperty> hproplist_t;
typedef std::pair<vr::TrackedDeviceIndex_t, vr::ETrackedDeviceClass> hdevpair_t;
typedef std::vector<hdevpair_t> hdevlist_t;

//  globals
//------------------------------------------------------------------------------
//  Eye nomenclature
constexpr const char* LEYE = "Left";
constexpr const char* REYE = "Right";
extern const heyes_t EYES;

//  generic functions
//------------------------------------------------------------------------------
//  Return {<str:base_name>, <str:type_name>, <enum:type>, <bool:array>}
std::tuple<std::string, std::string, vr::PropertyTypeTag_t, bool>
parse_prop_name(const std::string& pname);
