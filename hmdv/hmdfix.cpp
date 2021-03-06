/******************************************************************************
 * HMDQ Tools - tools for VR headsets and other hardware introspection        *
 * https://github.com/risa2000/hmdq                                           *
 *                                                                            *
 * Copyright (c) 2019, Richard Musil. All rights reserved.                    *
 *                                                                            *
 * This source code is licensed under the BSD 3-Clause "New" or "Revised"     *
 * License found in the LICENSE file in the root directory of this project.   *
 * SPDX-License-Identifier: BSD-3-Clause                                      *
 ******************************************************************************/

#include <iomanip>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "calcview.h"
#include "jkeys.h"
#include "misc.h"
#include "verhlp.h"

#include "fifo_map_fix.h"

//  locals
//------------------------------------------------------------------------------
//  miscellanous section keys
static constexpr auto MM_IN_METER = 1000;

//  fix identifications
//------------------------------------------------------------------------------
static constexpr const char* PROG_VER_DATETIME_FORMAT_FIX = "0.3.1";
static constexpr const char* PROG_VER_OPENVR_SECTION_FIX = "1.0.0";
static constexpr const char* PROG_VER_IPD_FIX = "1.2.3";
static constexpr const char* PROG_VER_FOV_FIX = "1.2.4";
static constexpr const char* PROG_VER_OPENVR_LOCALIZED = "1.3.4";

//  functions
//------------------------------------------------------------------------------
//  Return 'hmdv_ver' if it is defined in JSON data, otherwise return 'hmdq_ver'.
std::string get_hmdx_ver(const json& jd)
{
    const auto misc = jd[j_misc];
    std::string hmdx_ver;
    if (misc.contains(j_hmdv_ver)) {
        hmdx_ver = misc[j_hmdv_ver].get<std::string>();
    }
    else {
        hmdx_ver = misc[j_hmdq_ver].get<std::string>();
    }
    return hmdx_ver;
}

//  Fix datetime format in misc.time field (ver < v0.3.1)
void fix_datetime_format(json& jd)
{
    std::istringstream stime;
    std::tm tm = {};
    // get the old format with 'T' in the middle
    stime.str(jd[j_misc][j_time].get<std::string>());
    stime >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    // put the new format without 'T'
    jd[j_misc][j_time] = fmt::format("{:%F %T}", tm);
}

//  Fix for moved OpenVR things from 'misc' to 'openvr'
void fix_misc_to_openvr(json& jd)
{
    json jopenvr;
    if (jd[j_misc].contains("openvr_ver")) {
        jopenvr[j_rt_ver] = jd[j_misc]["openvr_ver"];
        jd[j_misc].erase("openvr_ver");
    }
    else {
        jopenvr[j_rt_ver] = "n/a";
    }
    jopenvr[j_rt_path] = "n/a";
    jd[j_openvr] = jopenvr;
}

void fix_ipd_unit(json& jd)
{
    // the old IPD is in milimeters, transform it to meters
    const auto ipd = jd[j_geometry][j_view_geom][j_ipd].get<double>() / MM_IN_METER;
    jd[j_geometry][j_view_geom][j_ipd] = ipd;
}

//  Fix for vertical FOV calculation in (ver < v1.2.4)
void fix_fov_calc(json& jd)
{
    const auto fov_tot = calc_total_fov(jd[j_geometry][j_fov_head]);
    jd[j_geometry][j_fov_tot] = fov_tot;
}

//  move openvr data into openvr section (ver < v1.3.4)
void fix_openvr_section(json& jd)
{
    jd[j_openvr][j_devices] = jd[j_devices];
    jd[j_openvr][j_properties] = jd[j_properties];
    jd[j_openvr][j_geometry] = jd[j_geometry];
    jd.erase(j_devices);
    jd.erase(j_properties);
    jd.erase(j_geometry);
}

//  Check and run all fixes (return true if there was any)
bool apply_all_relevant_fixes(json& jd)
{
    const auto hmdx_ver = get_hmdx_ver(jd);
    bool fixed = false;
    // datetime format fix - remove 'T' in the middle
    if (comp_ver(hmdx_ver, PROG_VER_DATETIME_FORMAT_FIX) < 0) {
        fix_datetime_format(jd);
        fixed = true;
    }
    // moved OpenVR things from 'misc' to 'openvr'
    if (comp_ver(hmdx_ver, PROG_VER_OPENVR_SECTION_FIX) < 0) {
        fix_misc_to_openvr(jd);
        fixed = true;
    }
    // change IPD unit in the JSON file - mm -> meters
    if (comp_ver(hmdx_ver, PROG_VER_IPD_FIX) < 0) {
        fix_ipd_unit(jd);
        fixed = true;
    }
    // change the vertical FOV calculation formula
    if (comp_ver(hmdx_ver, PROG_VER_FOV_FIX) < 0) {
        fix_fov_calc(jd);
        fixed = true;
    }
    // move all OpenVR data into 'openvr' section
    if (comp_ver(hmdx_ver, PROG_VER_OPENVR_LOCALIZED) < 0) {
        fix_openvr_section(jd);
        fixed = true;
    }
    // add 'hmdv_ver' into misc, if some change was made
    if (fixed) {
        jd[j_misc][j_hmdv_ver] = PROG_VERSION;
    }
    return fixed;
}
