# wxx dependencies

|project|license [^_l]|description [dependencies]|version|source|diff [^_d]|
|-------|-------------|--------------------------|-------|------|----------|
|<a id='wxx' />[wxx](https://github.com/externpro/wxx)|[wxWindows](http://wxcode.sourceforge.net/ 'wxWindows Library License')|wxWidget-based extra components [deps: _wxwidgets_]| | |  [intro]|
|<a id='wxwidgets' />[wxwidgets](http://wxwidgets.org/)|[wxWindows](https://wxwidgets.org/about/licence/ 'essentially LGPL with an exception')|Cross-Platform C++ GUI Library|[xpv3.1.0.6](https://github.com/externpro/wxWidgets/releases/tag/xpv3.1.0.6 'release')|[repo](https://github.com/externpro/wxWidgets 'github.com/externpro/wxWidgets') [upstream](https://github.com/wxWidgets/wxWidgets 'github.com/wxWidgets/wxWidgets')|[diff](https://github.com/externpro/wxWidgets/compare/v3.1.0...xpv3.1.0.6 'github.com/externpro/wxWidgets/compare/v3.1.0...xpv3.1.0.6') [intro(msw), native(unix)]|

![deps](xprodeps.svg 'dependencies')

Dependency version check: all 1 parent-manifest versions match pinned versions.

|diff  |description|
|------|-----------|
|patch |diff modifies/patches existing cmake|
|intro |diff introduces cmake|
|auto  |diff adds cmake to replace autotools/configure/make|
|native|diff adds cmake but uses existing build system|
|bin   |diff adds cmake to repackage binaries built elsewhere|
|fetch |diff adds cmake and utilizes FetchContent|

[^_l]: see [SPDX License List](https://spdx.org/licenses/ '') for a list of commonly found licenses
[^_d]: see table above with description of diff
