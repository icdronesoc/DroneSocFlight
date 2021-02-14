import subprocess, os

Import("env")


def find_file_on_system_path(file):
    paths = os.environ['PATH'].split(os.pathsep)
    if not os.path.isfile(file):
        for p in paths:
            f = os.path.join(p, file)
            if os.path.isfile(f):
                return f
        return None
    else:
        return file


# Re-run NanoPB generator if we've been told where it is
nanoPbPath = find_file_on_system_path("nanopb_generator.py")
if nanoPbPath is not None:
    print("Running NanoPB Generator.")
    os.chdir("src")
    os.system("python " + nanoPbPath + " config/Config.proto")
    os.chdir("../")
else:
    print("NanoPB Generator not detected.")

# Calculate git revision
gitRevision = (
    subprocess.check_output(["git", "rev-parse", "HEAD"])
        .strip()
        .decode("utf-8")
)

# Calculate build target include file
targetName = env.get("PIOENV")
buildTarget = "hardware/target/" + targetName + ".h"

env.Append(CPPDEFINES=[
    ("STRINGIFY2(X)", "#X"),
    ("STRINGIFY(X)", "STRINGIFY2(X)"),
    ("BUILD_GIT_REVISION", gitRevision),
    ("BUILD_TARGET", buildTarget)
])
