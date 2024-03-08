#!/usr/bin/env python3

import os
import re
import shutil

from upstream_utils import (
    download_release,
)


def main():
    upstream_root = download_release(
        "https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.2.0.tar.gz",
        "llhttp-release-v9.2.0",
    )
    wpilib_root = get_repo_root()
    wpinet = os.path.join(wpilib_root, "wpinet")

    # Copy header into allwpilib
    dest_filename = os.path.join(
        wpinet, "src/main/native/thirdparty/llhttp/include/wpi/expected"
    )
    shutil.copyfile(
        os.path.join(upstream_root, "include/tl/expected.hpp"), dest_filename
    )

    # Rename namespace from tl to wpi
    with open(dest_filename) as f:
        content = f.read()
    content = content.replace("namespace tl", "namespace wpi")
    content = content.replace("tl::", "wpi::")
    content = content.replace("TL_", "WPI_")
    with open(dest_filename, "w") as f:
        f.write(content)


if __name__ == "__main__":
    main()
