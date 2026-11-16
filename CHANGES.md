# Changelog

All notable user-facing changes to GLFW-OCaml are documented in this file. The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

See the [GLFW changelog](https://www.glfw.org/changelog.html) for the features added by new versions of GLFW.

## 3.5 (unreleased)

This release updates the binding to GLFW 3.5; see the [GLFW release notes](https://https://www.glfw.org/docs/3.5/news.html) for the features added in this version of GLFW.

## 3.4

This release updates the binding to GLFW 3.4; see the [GLFW release notes](https://https://www.glfw.org/docs/3.4/news.html) for the features added in this version of GLFW.

### Added

- Support for the new GLFW 3.4 features, including the new exceptions (`CursorUnavailable`, `FeatureUnavailable`, `FeatureUnimplemented`, `PlatformUnavailable`), the new init hints (`Platform`, `AnglePlatformType`, `X11XcbVulkanSurface`, `WaylandLibdecor`), the new window hints and attributes (e.g. `MousePassthrough`, `PositionX` and `PositionY`, `ScaleFramebuffer`, `Win32KeyboardMenu`, `Win32ShowDefault`, `WaylandAppID`), the new cursor shapes and the `Captured` cursor mode, named gamepad buttons and axes (`gamepad_button_*`, `gamepad_axis_*`) and the new functions `getPlatform`, `platformSupported` and `getWindowTitle`.

### Changed

- Flag lists (key modifiers, joystick hats) are now returned in ascending order.
- `getGamepadState` now raises `InvalidValue` when the joystick is not connected or is not a gamepad, instead of returning meaningless data.
- `getPrimaryMonitor` now raises `Failure` when there is no primary monitor.
- `getKey` and `getKeyScancode` now raise `Invalid_argument` when called with `Unknown`.
- `Image.create` now requires strictly positive dimensions.
- The compatibility of the installed GLFW package version is now checked with pkg-config at build time.

### Fixed

- The window attributes `DoubleBuffer`, `ContextDebug`, `ContextNoError` and `ContextReleaseBehavior` can now be read with `getWindowAttrib`.
- The missing key modifiers `CapsLock` and `NumLock` have been added.
- The joystick callback now reports `Connected` and `Disconnected` correctly.
- `getWindowAttrib` no longer fails on the `ContextReleaseBehavior` attribute.
- The dimension check in `Image.create` can no longer overflow.
- `getClipboardString` no longer crashes when GLFW returns `NULL`.
- Joysticks with many buttons or hats are now handled correctly.
- Memory allocation failures now raise `OutOfMemory` instead of crashing.
- An exception raised inside a callback is now reported as a fatal uncaught exception instead of escaping into C code.
- Calling `terminate` without having destroyed every window first no longer leaks memory.
- The error callback no longer risks using an invalidated value if a garbage collection happens at the wrong time.

### Requirements

- OCaml >= 4.03.0
- dune >= 3.1

### Known limitations

- `getPrimaryMonitor` and `getGamepadState` raise exceptions instead of returning an option type; this will be fixed in the next major version (see the README).
