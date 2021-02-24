@echo off

echo Downloading or updating MAVLink...
if exist mavlink-src\.git\ (
    cd mavlink-src
    git fetch
    git checkout origin/master
    cd ../
) else (
    git clone https://github.com/mavlink/mavlink.git --recursive mavlink-src
)

set PYTHONPATH=%CD%\mavlink-src

echo Running MAVLink generator...
REM Validation requires lxml, so we use --no-validate to skip having to install it.
python -m pymavlink.tools.mavgen --lang=C++11 --wire-protocol=2.0 --output=src/mavlink/ mavlink-src/message_definitions/v1.0/standard.xml --no-validate
