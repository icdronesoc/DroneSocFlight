import subprocess
Import("env")

gitRevision = (
    subprocess.check_output(["git", "rev-parse", "HEAD"])
        .strip()
        .decode("utf-8")
)

targetName = env.get("PIOENV")
buildTarget = "hardware/target/" + targetName + ".h"

env.Append(CPPDEFINES=[
    ("STRINGIFY2(X)", "#X"),
    ("STRINGIFY(X)", "STRINGIFY2(X)"),
    ("BUILD_GIT_REVISION", gitRevision),
    ("BUILD_TARGET", buildTarget)
])
