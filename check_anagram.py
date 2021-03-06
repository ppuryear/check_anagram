#!/usr/bin/env python3
#
# Copyright 2012 Philip Puryear
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import re

def setup_argparse():
    """Build an ArgumentParser object for this program."""
    parser = argparse.ArgumentParser(
            description='Prints "Yes" if string1 and string2 are anagrams, "No" otherwise.')
    parser.add_argument('string1', help='The first string.')
    parser.add_argument('string2', help='The second string.')
    parser.add_argument('-c', '--keep-case', action='store_true',
            help='Do not ignore case.')
    parser.add_argument('-w', '--keep-nonword', action='store_true',
            help='Do not ignore non-word characters.')
    return parser

def check_anagrams(string1, string2):
    # Fast path: two strings cannot be anagrams if their lengths differ.
    if len(string1) != len(string2):
        return False
    return sorted(string1) == sorted(string2)

def main():
    args = setup_argparse().parse_args()

    def strip(input):
        if not args.keep_case:
            input = input.lower()
        if not args.keep_nonword:
            input = re.sub('\\W', '', input)
        return input

    if check_anagrams(strip(args.string1), strip(args.string2)):
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()

