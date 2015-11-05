# -*- coding: utf-8 -*-

# Copyright (c) 2015, Michael Droettboom All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:

# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# The views and conclusions contained in the software and
# documentation are those of the authors and should not be interpreted
# as representing official policies, either expressed or implied, of
# the FreeBSD Project.

from __future__ import print_function, unicode_literals, absolute_import

import glob
import imp
import os
import sys


def write_if_different(filename, data):
    """ Write `data` to `filename`, if the content of the file is different.

    Parameters
    ----------
    filename : str
        The file name to be written to.
    data : bytes
        The data to be written to `filename`.
    """
    assert isinstance(data, bytes)

    if os.path.exists(filename):
        with open(filename, 'rb') as fd:
            original_data = fd.read()
    else:
        original_data = None

    if original_data != data:
        with open(filename, 'wb') as fd:
            fd.write(data)


if sys.version_info[0] >= 3:
    from io import StringIO
    string_types = (str, bytes)

    def get_byte(x):
        return x
else:
    from cStringIO import StringIO
    string_types = (str, unicode)

    def get_byte(x):
        return ord(x)

def generate_c_docstrings(name, input_path, output_path):
    input_file_path = os.path.join(input_path, name + ".py")
    module = imp.load_source(name, input_file_path)
    docstrings = module.__dict__

    keys = [
        key for key in docstrings.keys()
        if not key.startswith('__') and type(key) in string_types and
        type(docstrings[key]) in string_types]
    keys.sort()
    docs = {}
    for key in keys:
        docs[key] = docstrings[key].encode('utf8').lstrip() + b'\0'

    h_file = StringIO()
    h_file.write("""/*
DO NOT EDIT!

This file is autogenerated by setup.py.  To edit its contents, edit
{0}.
*/

#ifndef __DOC_{1}_H__
#define __DOC_{1}_H__
""".format(input_file_path, name.upper(), name))
    for key in keys:
        val = docs[key]
        h_file.write('extern char doc_{0}[{1}];\n'.format(key, len(val)))
    h_file.write("\n#endif\n\n")

    write_if_different(
        os.path.join(output_path, name + ".h"),
        h_file.getvalue().encode('utf-8'))

    c_file = StringIO()
    c_file.write("""/*
DO NOT EDIT!

This file is autogenerated by setup.py.  To edit its contents, edit
{0}

We use numeric array initializers rather than string literals here,
because some C compilers, notably MSVC, do not support string literals
greater than 256 characters.
*/

#include "{1}.h"

""".format(input_file_path, name))

    for key in keys:
        val = docs[key]
        c_file.write('char doc_{0}[{1}] = {{\n'.format(
            key, len(val)))
        for i in range(0, len(val), 10):
            c_file.write('    ')
            for c in val[i:i+10]:
                c_file.write("0x{0:02x}, ".format(get_byte(c)))
            c_file.write('\n')
        c_file.write('};\n\n')

    write_if_different(
        os.path.join(output_path, name + ".c"),
        c_file.getvalue().encode('utf-8'))


def convert_all_docstrings(input_dir, output_dir):
    sys.path.insert(0, input_dir)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for path in glob.glob(os.path.join(input_dir, "*.py")):
        name = os.path.basename(path)[:-3]
        module = imp.load_source(name, path)
        generate_c_docstrings(name, input_dir, output_dir)
