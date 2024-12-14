// Copyright Cartesi and individual authors (see AUTHORS)
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along
// with this program (see COPYING). If not, see <https://www.gnu.org/licenses/>.
//

#ifndef CM_MACHINE_C_VERSION_H // NOLINTBEGIN
#define CM_MACHINE_C_VERSION_H

#define CM_MARCHID EMULATOR_MARCHID

#define CM_VERSION_MAJOR EMULATOR_VERSION_MAJOR
#define CM_VERSION_MINOR EMULATOR_VERSION_MINOR
#define CM_VERSION_PATCH EMULATOR_VERSION_PATCH
#define CM_VERSION_LABEL "EMULATOR_VERSION_LABEL"

#define _CM_STR_HELPER(x) #x
#define _CM_STR(x) _CM_STR_HELPER(x)
#define CM_VERSION                                                                                                     \
    _CM_STR(CM_VERSION_MAJOR) "." _CM_STR(CM_VERSION_MINOR) "." _CM_STR(CM_VERSION_PATCH) CM_VERSION_LABEL
#define CM_VERSION_MAJMIN _CM_STR(CM_VERSION_MAJOR) "." _CM_STR(CM_VERSION_MINOR)

#define CM_MIMPID (CM_VERSION_MAJOR * 1000 + CM_VERSION_MINOR)

#endif // NOLINTEND
