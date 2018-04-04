/**
 * Copyright (c) 2015-2018, The Kovri I2P Router Project
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "core/router/info.h"

namespace core = kovri::core;

BOOST_AUTO_TEST_SUITE(RouterInfoCaps)

BOOST_AUTO_TEST_CASE(Repeated)
{
  core::RouterInfo ri;

  ri.SetCaps({
      core::RouterInfo::Cap::BWUnlimited,
      core::RouterInfo::Cap::BWUnlimited,
      core::RouterInfo::Cap::Floodfill,
      core::RouterInfo::Cap::Floodfill,
      core::RouterInfo::Cap::Reachable,
      core::RouterInfo::Cap::Reachable,
  });

  // Impl adds O for backwards compat
  BOOST_CHECK_EQUAL(ri.get_caps(), "XOfR");
}

BOOST_AUTO_TEST_CASE(OutOfOrder)
{
  core::RouterInfo ri;

  ri.SetCaps({
      core::RouterInfo::Cap::Reachable,
      core::RouterInfo::Cap::Floodfill,
      core::RouterInfo::Cap::BWUnlimited,
  });

  BOOST_CHECK_EQUAL(ri.get_caps(), "XOfR");
}

BOOST_AUTO_TEST_CASE(Invalid)
{
  core::RouterInfo ri;

  ri.SetCaps("abc");
  ri.SetCaps("123");

  BOOST_CHECK_EQUAL(ri.get_caps(), "");
}

BOOST_AUTO_TEST_CASE(MixedValidAndInvalid)
{
  core::RouterInfo ri;

  ri.SetCaps({core::RouterInfo::Cap::BW2000});
  ri.SetCaps("abc");
  ri.SetCaps({core::RouterInfo::Cap::Reachable});
  ri.SetCaps("123");

  BOOST_CHECK_EQUAL(ri.get_caps(), "POR");
}

BOOST_AUTO_TEST_CASE(HasCaps)
{
  core::RouterInfo ri;

  ri.SetCaps({core::RouterInfo::Cap::SSUTesting,
              core::RouterInfo::Cap::SSUIntroducer});

  BOOST_CHECK(ri.HasCaps({core::RouterInfo::Cap::SSUTesting,
                          core::RouterInfo::Cap::SSUIntroducer}));
}

BOOST_AUTO_TEST_CASE(RemoveCaps)
{
  core::RouterInfo ri;

  ri.SetCaps({core::RouterInfo::Cap::BW256,
              core::RouterInfo::Cap::BW128,
              core::RouterInfo::Cap::Reachable});

  ri.RemoveCaps({core::RouterInfo::Cap::BW256});
  BOOST_CHECK_EQUAL(ri.get_caps(), "NR");

  ri.RemoveCaps("N");
  BOOST_CHECK_EQUAL(ri.get_caps(), "R");
}

BOOST_AUTO_TEST_SUITE_END()
