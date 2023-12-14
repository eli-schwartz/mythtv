# smolt - Fedora hardware profiler
#
# Copyright (C) 2011 Raymond Wagner <sebastian@pipping.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

# This sets up a factory for urllib2.Request objects, automatically
# providing the base url, user agent, and proxy information.
# The object returned is slightly modified, with a shortcut to urlopen.

import urllib.request, urllib.error, urllib.parse
import urllib.parse

class _Request( urllib.request.Request ):
    timeout = None
    def open(self):
        if self.timeout:
            return urllib.request.urlopen(self, None, self.timeout)
        return urllib.request.urlopen(self)

class _RequestFactory:
    def __init__(self, baseurl, user_agent, timeout, proxy):
        self.base_url = baseurl
        self.user_agent = user_agent
        self.timeout = timeout
        self.proxy = proxy

    def __call__(self, *args, **kwargs):
        return self.new_request(*args, **kwargs)

    def new_request(self, url):
        url = urllib.parse.urljoin(self.base_url, url)
        req = _Request(url)
        req.timeout = self.timeout
        if self.proxy:
            req.set_proxy(self.proxy, 'http')
        if self.user_agent:
            req.add_header('User-Agent', self.user_agent)
        return req

_request = None

def ConnSetup(baseurl, user_agent=None, timeout=120, proxy=None):
    global _request
    if _request is None:
        _request = _RequestFactory(baseurl, user_agent, timeout, proxy)

def Request(url=None):
    global _request
    if _request is None:
        raise Exception("Request Factory not yet spawned")
    if url:
        return _request(url)
    return _request.base_url
