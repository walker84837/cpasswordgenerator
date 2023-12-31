# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## \[Unreleased\]

## \[0.2.1\] - 2023-12-16

### Fixed

  - Fixed a bug where the `get_random_int()` function would generate random
    numbers incorrectly.
  - The code style is now more consistent, using the Linux kernel coding style.

## \[0.2.0\] - 2023-11-27

### Removed

  - Removed the usage of C's `rand()` function.

### Changed

  - Replaced C's `rand()` function with OpenSSL's random function for security purposes.

## \[0.1.0\] - 2023-11-02

### Added

  - Initial release of `cpasswordgenerator`.
