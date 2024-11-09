#!/usr/bin/env python3

import os
import shutil
import subprocess

from upstream_utils import Lib, copy_to


def run_global_replacements(wpiutil_upb_files):
    for wpi_file in wpiutil_upb_files:
        with open(wpi_file) as f:
            content = f.read()

        # Prefix all upb names with wpi
        content = content.replace("upb_", "wpi_upb_")
        content = content.replace("UPB_", "WPI_UPB_")

        # Fix #includes
        content = content.replace('include "upb.h"', 'include "wpi/upb.h"')
        content = content.replace('include "utf8_range.h"', 'include "wpi/utf8_range.h"')

        with open(wpi_file, "w") as f:
            f.write(content)


def copy_upstream_src(wpilib_root):
    upstream_root = os.path.abspath(".")
    wpiutil = os.path.join(wpilib_root, "wpiutil/src/main/native/thirdparty/upb")

    # Delete old install
    for d in ["src", "include"]:
        shutil.rmtree(os.path.join(wpiutil, d), ignore_errors=True)

    # Build the amalgamation
    subprocess.check_call(["bazelisk", "build", "--spawn_strategy", "local", "//upb:gen_amalgamation"])

    # Copy upb files into allwpilib
    os.chdir(os.path.join(upstream_root, "bazel-bin/upb"))
    copy_to(["upb.c"], os.path.join(wpiutil, "src"))
    copy_to(["upb.h"], os.path.join(wpiutil, "include/wpi"))

    # Copy utf8_range files into allwpilib
    os.chdir(os.path.join(upstream_root, "third_party/utf8_range"))
    copy_to(["utf8_range.c"], os.path.join(wpiutil, "src"))
    copy_to(["utf8_range.h"], os.path.join(wpiutil, "include/wpi"))

    # Do replacements
    run_global_replacements([
        os.path.join(wpiutil, "src", "upb.c"),
        os.path.join(wpiutil, "src", "utf8_range.c"),
        os.path.join(wpiutil, "include/wpi", "upb.h"),
        os.path.join(wpiutil, "include/wpi", "utf8_range.h"),
    ])


def main():
    name = "upb"
    url = "https://github.com/protocolbuffers/protobuf"
    tag = "v5.28.3"

    protobuf = Lib(name, url, tag, copy_upstream_src)
    protobuf.main()


if __name__ == "__main__":
    main()
