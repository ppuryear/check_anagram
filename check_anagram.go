// Copyright 2012 Philip Puryear
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package main

import (
	"flag"
	"fmt"
	"os"
	"regexp"
	"sort"
	"strings"
)

type ProgramArguments struct {
	KeepCase    bool
	KeepNonword bool
}

var gProgramArgs ProgramArguments

// Used to implement lexicographical string sorting.
type SortableRunes []rune

func (s SortableRunes) Len() int {
	return len(s)
}

func (s SortableRunes) Less(i, j int) bool {
	return s[i] < s[j]
}

func (s SortableRunes) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}

func CheckAnagrams(a, b string) bool {
	// Fast path: if a and b have different lengths, then they cannot be
	// anagrams.
	if len(a) != len(b) {
		return false
	}

	aRunes, bRunes := SortableRunes(a), SortableRunes(b)
	sort.Sort(aRunes)
	sort.Sort(bRunes)
	return string(aRunes) == string(bRunes)
}

func StripInput(input string) string {
	if !gProgramArgs.KeepCase {
		input = strings.ToLower(input)
	}
	if !gProgramArgs.KeepNonword {
		stripRegexp := regexp.MustCompile("\\W")
		return stripRegexp.ReplaceAllString(input, "")
	}
	return input
}

func Usage() {
	fmt.Fprint(os.Stderr,
		"usage: check_anagram [options] string1 string2\n\n" +
		"Prints \"Yes\" if string1 and string2 are anagrams, \"No\" otherwise.\n\n" +
		"Options:\n")
	flag.PrintDefaults()
}

func ParseCommandLine() {
	flag.Usage = Usage
	flag.BoolVar(&gProgramArgs.KeepCase, "keep-case", false,
		"do not ignore case")
	flag.BoolVar(&gProgramArgs.KeepNonword, "keep-nonword", false,
		"do not ignore non-word characters")
	flag.Parse()

	if flag.NArg() != 2 {
		fmt.Fprintln(os.Stderr, "incorrect number of inputs")
		Usage()
		os.Exit(1)
	}
}

func main() {
	ParseCommandLine()
	if CheckAnagrams(StripInput(flag.Arg(0)), StripInput(flag.Arg(1))) {
		fmt.Println("Yes")
	} else {
		fmt.Println("No")
	}
}
