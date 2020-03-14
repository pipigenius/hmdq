/******************************************************************************
 * HMDQ Tools - tools for an OpenVR HMD and other hardware introspection      *
 * https://github.com/risa2000/hmdq                                           *
 *                                                                            *
 * Copyright (c) 2020, Richard Musil. All rights reserved.                    *
 *                                                                            *
 * This source code is licensed under the BSD 3-Clause "New" or "Revised"     *
 * License found in the LICENSE file in the root directory of this project.   *
 * SPDX-License-Identifier: BSD-3-Clause                                      *
 ******************************************************************************/

#include <OVR_CAPI.h>

#include "jkeys.h"
#include "oculus_config.h"

namespace oculus {

//  OculusVR defaults
//------------------------------------------------------------------------------
static constexpr auto INIT_FLAGS = ovrInit_Invisible;

// clang-format off
static const json VERB_PROPS = {
    {"HmdType", 0},
    {"ProductName", 0},
    {"Manufacturer", 0},
    {"VendorId", 2},
    {"ProductId", 2},
    {"SerialNumber", 0},
    {"FirmwareMajor", 2},
    {"FirmwareMinor", 2},
    {"AvailableHmdCaps", 2},
    {"DefaultHmdCaps", 2},
    {"AvailableTrackingCaps", 2},
    {"DefaultTrackingCaps", 2},
    {"DisplayRefreshRate", 0},
};
// clang-format on

// clang-format off
//  currently identified properties with serial numbers
static const json ANON_PROPS = {
    "SerialNumber",
};
// clang-format on

//  OculusVR Config class (default config)
//------------------------------------------------------------------------------
// Initialize the VR subsystem default config data
Config::Config()
{
    m_jConfig[j_init_flags] = INIT_FLAGS;
    m_jConfig[j_verbosity][j_properties] = VERB_PROPS;
    m_jConfig[j_anonymize][j_properties] = ANON_PROPS;
}

// Return VR subystem ID
std::string Config::get_id()
{
    return j_oculus;
}

// Return VR subystem default config
json& Config::get_data()
{
    return m_jConfig;
}

} // namespace oculus